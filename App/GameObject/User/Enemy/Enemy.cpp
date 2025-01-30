#include "Enemy.h"
#include "EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "Engine/GameObject/GameObjectManager.h"

void Enemy::Init()
{
	// 身体用トランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.translate_.y += 4.0f;
	bodyTransform_.SetParent(&transform_, 0b011);

	// 頭用トランスフォーム
	headTransform_.Init();
	headTransform_.translate_.y += 5.0f;
	headTransform_.SetParent(&transform_, 0b011);

	//武器用のトランスフォームの初期化
	weaponTransform_.Init();
	weaponTransform_.SetParent(&transform_);

	// 落ち影用トランスフォームの初期化
	shadowTransform_.Init();
	shadowTransform_.SetParent(&transform_);

#ifdef _DEBUG // デバッグ時のみ行う

	// デバッグ時は攻撃を行わないように
	isAttack_ = false;
	// 一度ダウンした場合起き上がらないように
	isNeverDown_ = false;
	// 無限HP
	isInfiniteHP_ = true;

#endif // _DEBUG

	// メッシュを追加
	AddSkiningModel("Enemy", &transform_, "./Resources/Enemy", "Enemy_Anim.gltf");
	AddSkiningModel("Sword", &weaponTransform_, "./Resources/EnemySword", "EnemySword.gltf");
	skiningModels_["Enemy"]->materials_[0].enableLighting_ = false;
	skiningModels_["Sword"]->materials_[0].enableLighting_ = false;

	// 落ち影用モデル
	AddNormalModel("Shadow", &shadowTransform_, "./Resources/DropShadow", "DropShadow.obj");
	normalModels_["Shadow"]->materials_[1].color_ = Vector4(0.0f, 0.0f, 0.0f, 0.5f);

	// 待機アニメーションの再生
	skiningModels_["Enemy"]->animationManager_.PlayAnimation("Idle", true);

	// 武器を左手に追従するようにする
	skiningModels_["Enemy"]->SetBoneParent("WeaponAnchor", &weaponTransform_);
	// 身体の辺り判定を上半身に追従させる
	skiningModels_["Enemy"]->SetBoneParent("LowerBody", &bodyTransform_);

	// 球のコライダー追加
	AddColliderSphere("Boss", &worldPos_, &colliderRadius_);
	// 頭のコライダー追加
	//AddColliderSphere("BossHead", &worldHeadPos_, &headColliderRadius_);
	// 近接攻撃用コライダー追加
	AddColliderSphere("EnemyAttackCollider", &attackColliderPosition_, &attackColliderRadius_);
	GetCollider("EnemyAttackCollider")->SetIsActive(false);

	// 行動変更
	ChangeState(std::make_unique<EnemyRoot>());
	// 行動変更タイマーリセット
	stateChangeTimer_.Start(kStateChangeCoolTime_);

	// 効果音読み込み
	damageSound_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/SwordDamage.mp3");

	// 敵パーティクル再生
	enemyParticle_ = ParticleManager::GetInstance()->CreateNewParticle("Enemy", "./Engine/Resource/System/Plane", "Plane.obj", 0.0f, true);
	enemyParticle_->model_->materials_[1].tex_ = TextureManager::Load("EnemyParticle.png");
	enemyParticle_->model_->materials_[1].enableLighting_ = false;
	enemyParticle_->transform_.SetParent(&bodyTransform_);
	enemyParticle_->transform_.translate_.y -= 1.5f;
	enemyParticle_->emitterDataBuffer_->data_->count = 25;
	enemyParticle_->emitterDataBuffer_->data_->frequency = 0.1f;
	enemyParticle_->emitterDataBuffer_->data_->frequencyTime = 0.1f;
}

void Enemy::Update()
{
	// ワールド座標の取得
	worldPos_ = bodyTransform_.GetWorldPos();
	worldHeadPos_ = headTransform_.GetWorldPos();
	// 敵のＹ軸回転から回転行列を生成
	Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(transform_.rotate_.y);
	// 近接攻撃コライダーの位置調整
	attackColliderPosition_ = bodyTransform_.GetWorldPos() + (kAttackColliderOffset * rotateMat);

	// 回転を行う場合
	if (!isRotateLock_) {
		// 差分ベクトルを求める
		toPlayerDistance_ = transform_.translate_ - playerPos_->translate_;

		// 目標角度の取得
		targetAngle_ = std::atan2(toPlayerDistance_.x, toPlayerDistance_.z);

		// 目標角度に向かって徐々に補間
		transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngle_, angleCorrectSpeed);
	}

	// 死亡演出の更新
	if (isDeadStaging_) {
		DeadCameraStaging();
	}

	// ヒットストップ中であれば更新関数呼び出しからの早期リターン
	if (isHitStop_) {
		HitStopUpdate();
		return;
	}

	// ゲーム開始状態でない場合早期リターン
	if (!gameManager_->GetIsGameStart()) {
		return;
	}

	// 行動状態の更新
	state_->Update();

	// 落ち影更新
	ShadowUpdate();

	// 行動変更処理
	ChangeStateUpdate();

	// ヒットクールタイムタイマー更新
	hitCoolTimeTimer_.Update();

	// ワールド座標の取得
	worldPos_ = bodyTransform_.GetWorldPos();
	worldHeadPos_ = headTransform_.GetWorldPos();
	// 敵のＹ軸回転から回転行列を生成
	rotateMat = Matrix4x4::MakeRotateY(transform_.rotate_.y);
	// 近接攻撃コライダーの位置調整
	attackColliderPosition_ = bodyTransform_.GetWorldPos() + (kAttackColliderOffset * rotateMat);
}

void Enemy::DisplayImGui()
{
	// 攻撃行動を行うかの切り替え
	ImGui::Checkbox("is Attack", &isAttack_);
	// 無限HPか
	ImGui::Checkbox("InfiniteHP", &isInfiniteHP_);
	// 一度ダウンしたら起き上がらないか
	ImGui::Checkbox("isNeverDown", &isNeverDown_);

	transform_.DisplayImGui();

	// 強制ダウン状態にする
	if (ImGui::Button("Down")) {
		ChangeState(std::make_unique<EnemyDown>());
	}

	bodyTransform_.DisplayImGuiWithTreeNode("BodyTransform");
	shadowTransform_.DisplayImGuiWithTreeNode("Shadow");

	ImGui::DragInt("HP", &hp_);

	// 落ち影関係の調整
	ImGui::DragFloat("MaxShadowScale", &maxShadowScale, 0.01f, 0.05f);
	ImGui::DragFloat("MinShadowScale", &minShadowScale, 0.01f, 0.01f);

	// プレイヤーとの距離を表示
	float d = Vector3::Length(toPlayerDistance_);
	ImGui::DragFloat("toPlayerDistance", &d);

	// フィールド外か
	ImGui::Checkbox("FieldOut", &isFieldOut_);
}

void Enemy::OnCollisionEnter(Collider* target, [[maybe_unused]] Collider* source)
{
	// 死亡している場合早期リターン
	if (isDead_) { return; }

	// 弾と衝突していたら
	if (target->GetColliderName() == "Bullet") {
		// 弾の取得
		EnemyBullet* b = GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet");

		// 弾がプレイヤーが跳ね返した状態でない場合早期リターン
		if (!b->GetIsReturn()) { return; }

		// ラリー回数が最大回数を超過している場合
		if (rallyCount_ > kMaxRallyCount_) {
			// 弾に命中パーティクル再生を指示
			b->PlayHitParticle();

			// ダウン状態に
			ChangeState(std::make_unique<EnemyDown>());

			// 衝突した弾を破壊
			b->DeleteBullet();

			// ラリー回数リセット
			rallyCount_ = 0;

			// 早期リターン
			return;
		}

		// 現在のラリー回数 ~ 最大ラリー回数までのランダムな値をとる
		int percent = KLib::Random(rallyCount_, kMaxRallyCount_);
		// 最大ラリー回数と一致していた場合
		if (percent == kMaxRallyCount_) {
			// 弾に命中パーティクル再生を指示
			b->PlayHitParticle();

			// 麻痺パーティクルの再生
			PlayStunParticle();

			// ダウン状態に
			ChangeState(std::make_unique<EnemyDown>());

			// 衝突した弾を破壊
			b->DeleteBullet();

			// ラリー回数リセット
			rallyCount_ = 0;

			// 早期リターン
			return;
		}

		/// ここまで来たら敵のラリー成功
		// 弾に命中パーティクル再生を指示
		b->PlayHitParticle();

		// ラリーアニメーションの再生
		skiningModels_["Enemy"]->animationManager_.PlayAnimation("Rally", 0.1f, false);
		skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(1.0f);

		// ラリー回数加算
		rallyCount_++;

		// 弾を弾く
		b->SetVelocity(true, rallyCount_);
	}
}

void Enemy::OnCollision(Collider* target, [[maybe_unused]] Collider* source)
{
	// 死亡している場合は早期リターン
	if (isDead_) {
		return;
	}

	// 衝突元が近接攻撃コライダーだった場合早期リターン
	if (source->GetColliderName() == "EnemyAttackCollider") { return; }

	// 剣と衝突していたら
	if (target->GetColliderName() == "Sword" || target->GetColliderName() == "PlayerCollider") {
		// 下記条件の場合早期リターン
		// 1. プレイヤーが攻撃中でない
		// 2. ヒットクールタイムが終了している
		// 3. 敵がダウン中
		// 4. 衝突元が近接攻撃用コライダーの場合
		if (!player_->GetIsAttacking() || !hitCoolTimeTimer_.GetIsFinish()) { return; }

		// ダウンさせる
		if (state_->GetStateName() != "Down") {
			// プレイヤーがカウンター状態でなければ早期リターン
			if (player_->GetStateName() != "Counter") { return; }

			// クールタイムタイマー開始
			hitCoolTimeTimer_.Start(kHitCoolTime_);

			// ヒットストップ開始
			player_->StartHitStop(0.5f);
			StartHitStop(0.5f);

			// ダメージパーティクルの再生
			PlayDamageParticle();

			// ダメージ効果音の再生
			Audio::GetInstance()->PlayWave(damageSound_);

			// ダウン状態に変更
			ChangeState(std::make_unique<EnemyDown>());

			// 早期リターン
			return;
		}

		// 無限HPでない場合のみHPを減らす
		if (!isInfiniteHP_) {
			// HPを減らす
			hp_ -= player_->GetComboManager()->GetDamage();
		}
		// クールタイムタイマー開始
		hitCoolTimeTimer_.Start(kHitCoolTime_);

		// プレイヤーに攻撃が命中したことを伝える
		player_->SetIsHit(true);

		// ダメージパーティクルの再生
		PlayDamageParticle();

		// とどめの場合固定秒数でヒットストップ
		if (hp_ <= 0) {
			player_->StartHitStop(finishHitStopTime_, deadStagingAnimSpeed_);
			StartHitStop(finishHitStopTime_, deadStagingAnimSpeed_);

			// ブラー演出を開始
			player_->GetFollowCamera()->StartBlur(finishBlurStagingTime_, finishBlurEndTime_, finishBlurStrength_);

			// 死亡時の演出用カメラのセットアップ
			SetUpDeadCameraStaging();

			// 全パーティクルの再生速度を設定
			ParticleManager::GetInstance()->SetTimeScale(deadStagingAnimSpeed_);
		}
		else {
			// ヒットストップ開始
			player_->StartHitStop(player_->GetComboManager()->GetHitStopTime());
			StartHitStop(player_->GetComboManager()->GetHitStopTime());
		}

		// ダメージアニメーションの再生
		skiningModels_["Enemy"]->animationManager_.PlayAnimation("Damage", false);

		// ダメージ効果音の再生
		Audio::GetInstance()->PlayWave(damageSound_);
	}

	// フィールドと衝突している場合
	if (target->GetColliderName() == "Level") {
		// フィールド内に存在する
		isFieldOut_ = false;
	}
}

void Enemy::OnCollisionExit(Collider* target, [[maybe_unused]] Collider* source)
{
	// 死亡していなければ
	if (isDead_) {
		return;
	}

	// フィールドから離れた場合
	if (target->GetColliderName() == "Level") {
		// フィールド外に出ている
		isFieldOut_ = true;
	}
}

void Enemy::ChangeState(std::unique_ptr<IEnemyState> newState)
{
	// 共通初期化を行う
	newState->PreInit(this);
	// 初期化を行う
	newState->Init();

	// 初期化した新しいステートを代入
	state_ = std::move(newState);
}

void Enemy::StartHitStop(const float hitStopTime, float timeScale)
{
	// ヒットストップ秒数が0以下の場合早期リターン
	if (hitStopTime <= 0.0f) { return; }

	// 再生中アニメーションの再生速度を調整
	skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(timeScale);
	// ヒットストップタイマー開始
	hitStopTimer_.Start(hitStopTime);
	// ヒットストップ中状態に
	isHitStop_ = true;
}

void Enemy::HitStopUpdate()
{
	// ヒットストップタイマー終了時
	if (hitStopTimer_.GetIsFinish()) {
		// 再生中アニメーションの再生速度リセット
		skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(1.0f);

		// ヒットストップ状態終了
		isHitStop_ = false;
	}

	// ヒットストップタイマーの更新
	hitStopTimer_.Update();
}

void Enemy::ShadowUpdate()
{
	// 敵の高さを元に影の大きさを調整
	float shadowScale = KLib::Lerp<float>(maxShadowScale, minShadowScale, transform_.translate_.y, maxHeight_);
	shadowTransform_.scale_ = { shadowScale, 1.0f, shadowScale };

	// 高さは自動調整
	shadowTransform_.translate_.y = -(transform_.GetWorldPos().y) + shadowHeight_;
}

void Enemy::SetUpDeadCameraStaging()
{
	// カメラの切り替え秒数を求める
	kCameraSwitchTime_ = finishHitStopTime_ / static_cast<float>(kDeadStagingCameraCount_);

	// カメラをカウント分生成
	for (int32_t i = 0; i < kDeadStagingCameraCount_; i++) {
		// 演出用カメラ生成
		Camera* stagingCamera = GameObjectManager::GetInstance()->CreateInstance<Camera>("deadStagingCamera", IObject::TagCamera);

		// カメラ座標を敵座標に一度合わせる
		stagingCamera->transform_ = transform_;

		// オフセット
		const Vector3 kOffset = { 0.0f, 1.0f, -15.0f };

		// 差分ベクトルを求める
		Vector3 distance = transform_.translate_ - playerPos_->translate_;

		// 目標角度の取得
		float rotateY = std::atan2(distance.x, distance.z);

		// 回転角をランダムに指定
		rotateY -= static_cast<float>(std::numbers::pi) / 2.0f;

		// 回転行列の生成
		Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(rotateY);

		// オフセットを求める
		Vector3 offset = kOffset * rotateMat;

		// カメラ座標を動かす
		stagingCamera->transform_.translate_ += offset;
		stagingCamera->transform_.rotate_.y = rotateY;

		// カメラ配列に追加
		deadStagingCameras_.push_back(stagingCamera);
	}

	// 演出用カメラに切り替え
	deadStagingCameras_[0]->UseThisCamera();

	// カメラ切り替えタイマーを開始
	switchCameraTimer_.Start(kCameraSwitchTime_);

	// 死亡時カメラ演出の開始
	isDeadStaging_ = true;
}

void Enemy::DeadCameraStaging()
{
	// カメラ切り替えタイマー終了時
	if (switchCameraTimer_.GetIsFinish()) {
		// 表示するカメラ番号の加算
		displayCameraNumber_++;

		// 表示カメラ番号が死亡演出用のカメラ数を超過しているときは早期リターン
		if (displayCameraNumber_ >= kDeadStagingCameraCount_) { 
			// 演出終了
			isDeadStaging_ = false;
			return; 
		}

		// そうではない場合配列内の該当するカメラを使用する
		deadStagingCameras_[displayCameraNumber_]->UseThisCamera();
		// カメラ切り替えタイマーを再度開始
		switchCameraTimer_.Start(kCameraSwitchTime_);
	}

	deadStagingCameras_[displayCameraNumber_]->fov_ += deadstagingAddFOV_;

	// タイマーの更新
	switchCameraTimer_.Update();
}

void Enemy::PlayStunParticle()
{
	// 麻痺パーティクル再生
	stunParticle_ = ParticleManager::GetInstance()->CreateNewParticle("BulletSpark", "./Engine/Resource/System/Plane", "Plane.obj", 0.0f, true);
	stunParticle_->model_->materials_[1].tex_ = TextureManager::Load("BulletSparkParticle.png");
	stunParticle_->model_->materials_[1].enableLighting_ = false;
	stunParticle_->transform_.SetParent(&bodyTransform_);
	stunParticle_->emitterDataBuffer_->data_->count = 3;
	stunParticle_->emitterDataBuffer_->data_->frequency = 0.01f;
	stunParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;
}

void Enemy::ResetStunParticle()
{
	// スタンパーティクルインスタンスをnullptrに
	stunParticle_ = nullptr;
}

void Enemy::PlayDamageParticle()
{
	// 命中パーティクル再生
	Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("Hit", "./Engine/Resource/System/Plane", "Plane.obj", 0.5f);
	hit->model_->materials_[1].tex_ = TextureManager::Load("HitEffect.png");
	hit->model_->materials_[1].enableLighting_ = false;
	hit->transform_.SetParent(&bodyTransform_);
	hit->emitterDataBuffer_->data_->count = 1;
	hit->emitterDataBuffer_->data_->frequency = 1.0f;
	hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;

	// 命中破片パーティクル再生
	Particle* hitDebris = ParticleManager::GetInstance()->CreateNewParticle("HitDebris", "./Engine/Resource/System/Plane", "Plane.obj", 1.0f);
	hitDebris->model_->materials_[1].tex_ = TextureManager::Load("HitDebrisEffect.png");
	hitDebris->model_->materials_[1].enableLighting_ = false;
	hitDebris->transform_.SetParent(&bodyTransform_);
	hitDebris->emitterDataBuffer_->data_->count = 10;
	hitDebris->emitterDataBuffer_->data_->frequency = 3.0f;
	hitDebris->emitterDataBuffer_->data_->frequencyTime = 5.0f;
}

void Enemy::ChangeStateUpdate()
{
	// HPが0になっている場合
	if (hp_ <= 0 && !isDeadStaging_) {
		// 死亡時のステートへ変更
		if (state_->GetStateName() != "Dead") {
			ChangeState(std::make_unique<EnemyDead>());
		}

		// 永続パーティクルの終了
		if (enemyParticle_ != nullptr) {
			enemyParticle_->SetIsEnd(true);
			enemyParticle_ = nullptr;
		}

		// 早期リターン
		return;
	}

	// 待機状態である場合
	if (state_->GetStateName() == "Root") {
		// プレイヤーとの距離ベクトルの長さを求める
		float distance = Vector3::Length(toPlayerDistance_);

		// 距離が一定値以下であれば
		if (distance <= minPlayerDistance_) {
			if (moveCount_ >= maxMoveCount_) {
				// 敵を隠し、距離を取らせる
				ChangeState(std::make_unique<EnemyHide>());
				// 移動状態のカウントをリセットする
				moveCount_ = 0;
			}
			else {
				// 敵が移動する
				ChangeState(std::make_unique<EnemyMove>());
				// 移動状態のカウントを加算する
				moveCount_++;
			}
		}
	}

	// 行動変更タイマーが終了している場合
	if (stateChangeTimer_.GetIsFinish()) {
		// 待機状態でない場合早期リターン
		if (state_->GetStateName() != "Root") {
			return;
		}

		// 攻撃可能状態で無ければリターン
		if (!canAttack_ || !isAttack_) {
			return;
		}

		int random = KLib::Random(0, 1);

		// 弾が生成されていない状態の場合
		if (GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") == nullptr) {
			if (random == 0) {
				// 背面攻撃を行う
				ChangeState(std::make_unique<EnemyHideAttack>());
				
			}
			else {
				// 敵が弾を撃つ
				ChangeState(std::make_unique<EnemyShot>());
			}
			
			rallyCount_ = 0;
			// 行動変更タイマーリセット
			stateChangeTimer_.Start(kStateChangeCoolTime_);
			// 移動カウントをリセット
			moveCount_ = 0;
		}
	}

	// 行動変更クールタイムタイマー更新
	if (state_->GetStateName() == "Root"
		&& GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") == nullptr
		&& GameObjectManager::GetInstance()->GetGameObject<Player>("Player")->GetStateName() != "Damage"
		&& isAttack_
		) {
		// 行動変更クールタイムの更新
		stateChangeTimer_.Update();
	}
}
