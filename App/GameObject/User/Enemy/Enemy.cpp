#include "Enemy.h"
#include "EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "Engine/GameObject/GameObjectManager.h"

void Enemy::Init()
{
	// アニメーションマネージャの取得
	animManager_ = AnimationManager::GetInstance();

	// 各種ワールドトランスフォームの初期化
	bodyTransform_.Init();
	// 親子付け
	bodyTransform_.SetParent(&transform_, 0b011);
	headTransform_.Init();
	headTransform_.SetParent(&bodyTransform_, 0b111);
	headTransform_.translate_ = { 0.0f, 1.1f, 0.0f };
	armTransform_R_.Init();
	armTransform_R_.SetParent(&bodyTransform_, 0b111);
	armTransform_R_.translate_ = { 0.0f, 1.0f, 0.0f };
	armTransform_L_.Init();
	armTransform_L_.SetParent(&bodyTransform_, 0b111);
	armTransform_L_.translate_ = { 0.0f, 1.0f, 0.0f };

	// 落ち影用トランスフォームの初期化
	shadowTransform_.Init();
	shadowTransform_.SetParent(&transform_);

	// アニメーションパラメータの生成

	// 待機状態
	CreateParameter("Enemy_Idle");
	// 弾発射までのチャージアニメーション
	CreateParameter("Enemy_Charge");
	// 弾発射までのチャージ中
	CreateParameter("Enemy_Charging");
	// 弾発射のアニメーション
	CreateParameter("Enemy_Shot");
	// 弾の跳ね返しアニメーション
	CreateParameter("Enemy_Rally");
	// ダウンモーション
	CreateParameter("Enemy_Down");
	// ダウン中モーション
	CreateParameter("Enemy_Downing");
	// ダメージアニメーション
	CreateParameter("Enemy_Damage");
	// 死亡アニメーション
	CreateParameter("Enemy_Dead");

	// アニメーションの作成
	enemyAnim_ = AnimationManager::GetInstance()->CreateAnimation("EnemyAnimation", "Enemy_Idle");
	enemyAnim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Head_Scale", &headTransform_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Head_Rotate", &headTransform_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Head_Translate", &headTransform_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_R_Scale", &armTransform_R_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_R_Rotate", &armTransform_R_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_R_Translate", &armTransform_R_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_L_Scale", &armTransform_L_.scale_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_L_Rotate", &armTransform_L_.rotate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Arm_L_Translate", &armTransform_L_.translate_);
	enemyAnim_->AddAnimationKeys<Vector3>("Translate", &transform_.translate_);
	enemyAnim_->AddAnimationKeys <float> ("Alpha", &color_.w);

#ifdef _DEBUG // デバッグ時のみ行う

	// デバッグ時は攻撃を行わないように
	isAttack_ = false;
	// 一度ダウンした場合起き上がらないように
	isNeverDown_ = false;
	// 無限HP
	isInfiniteHP_ = true;

#endif // _DEBUG

	// ループ状態にする
	enemyAnim_->isLoop_ = true;
	// この状態で再生
	enemyAnim_->Play();

	// メッシュを追加
	AddNormalModel("Body", &bodyTransform_, "./Resources/Enemy", "Body.obj");
	AddNormalModel("Head", &headTransform_, "./Resources/Enemy", "Head.obj");
	AddNormalModel("Arm_R", &armTransform_R_, "./Resources/Enemy", "Arm_R.obj");
	AddNormalModel("Arm_L", &armTransform_L_, "./Resources/Enemy", "Arm_L.obj");

	// 落ち影用モデル
	AddNormalModel("Shadow", &shadowTransform_, "./Resources/DropShadow", "DropShadow.obj");
	normalModels_["Shadow"]->materials_[1].color_ = Vector4(0.0f, 0.0f, 0.0f, 0.5f);

	// 球のコライダー追加
	AddColliderSphere("Boss", &worldPos_, &colliderRadius_);

	// 行動変更
	ChangeState(std::make_unique<EnemyRoot>());
	// 行動変更タイマーリセット
	stateChangeTimer_.Start(kStateChangeCoolTime_);

	// 効果音読み込み
	damageSound_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/SwordDamage.mp3");

	// 敵パーティクル再生
	enemyParticle_ = ParticleManager::GetInstance()->CreateNewParticle("Enemy", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
	enemyParticle_->model_->materials_[1].tex_ = TextureManager::Load("EnemyParticle.png");
	enemyParticle_->model_->materials_[1].enableLighting_ = false;
	enemyParticle_->transform_.SetParent(&bodyTransform_);
	enemyParticle_->transform_.translate_.y -= -0.9f;
	enemyParticle_->emitterDataBuffer_->data_->count = 25;
	enemyParticle_->emitterDataBuffer_->data_->frequency = 0.1f;
	enemyParticle_->emitterDataBuffer_->data_->frequencyTime = 0.1f;
}

void Enemy::Update()
{
	// ダウン状態でない場合
	if (state_->GetStateName() != "Down") {
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

	// ダウン状態および射撃状態で無い、かつ同じ行動状態でない場合
	std::string stateName = state_->GetStateName();
	if (stateName != "Down" && stateName != "Shot" && stateName != "Move") {
		// プレイヤーとの距離ベクトルの長さを求める
		float distance = Vector3::Length(toPlayerDistance_);

		// 距離が一定値以下であれば
		if (distance <= minPlayerDistance_) {
			// 敵が移動する
			ChangeState(std::make_unique<EnemyMove>());
		}
	}

	// 攻撃を行う状態でなければ早期リターン
	if (!isAttack_) {
		return;
	}

	// 行動変更タイマーが終了している場合
	if (stateChangeTimer_.GetIsFinish()) {
		// 待機状態でない場合早期リターン
		if (state_->GetStateName() == "Down" || state_->GetStateName() == "Shot" || state_->GetStateName() == "Move") {
			return;
		}

		// 攻撃可能状態で無ければリターン
		if (!canAttack_) {
			return;
		}

		// 弾が生成されていない状態の場合
		if (GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") == nullptr) {
			// 敵が弾を撃つ
			ChangeState(std::make_unique<EnemyShot>());
			rallyCount_ = 0;
			// 行動変更タイマーリセット
			stateChangeTimer_.Start(kStateChangeCoolTime_);
		}
	}

	// 行動変更クールタイムタイマー更新
	if (enemyAnim_->GetReadingParameterName() == "Enemy_Rally") {
		if (enemyAnim_->isLoop_) {
			enemyAnim_->isLoop_ = false;
		}

		if (enemyAnim_->isEnd_) {
			// 行動変更
			ChangeState(std::make_unique<EnemyRoot>());
		}
	}

	// ヒットクールタイムタイマー更新
	hitCoolTimeTimer_.Update();
	// 行動変更クールタイムタイマー更新
	if (state_->GetStateName() == "Root"
		&& GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") == nullptr
		&& GameObjectManager::GetInstance()->GetGameObject<Player>("Player")->GetStateName() != "Damage") {
		// 行動変更クールタイムの更新
		stateChangeTimer_.Update();
	}

	// ワールド座標の取得
	worldPos_ = transform_.GetWorldPos();
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
	headTransform_.DisplayImGuiWithTreeNode("HeadTransform");
	armTransform_R_.DisplayImGuiWithTreeNode("Arm_R_Transform");
	armTransform_L_.DisplayImGuiWithTreeNode("Arm_L_Transform");
	shadowTransform_.DisplayImGuiWithTreeNode("Shadow");

	enemyAnim_->DisplayImGui();

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

void Enemy::OnCollisionEnter(Collider* collider)
{
	// 死亡している場合早期リターン
	if (isDead_) { return; }

	// 弾と衝突していたら
	if (collider->GetColliderName() == "Bullet") {
		// 弾の取得
		EnemyBullet* b = GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet");

		// 弾がプレイヤーが跳ね返した状態でない場合早期リターン
		if (!b->GetIsReturn()) { return; }

		// 弾を跳ね返せたかどうかの一時フラグ
		bool isRally = false;

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

		// ループを切る
		enemyAnim_->isLoop_ = false;
		// アニメーションを変更
		enemyAnim_->ChangeParameter("Enemy_Rally", true);
		// ラリー回数加算
		rallyCount_++;
	}
}

void Enemy::OnCollision(Collider* collider)
{
	// 死亡している場合は早期リターン
	if (isDead_) {
		return;
	}

	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword" || collider->GetColliderName() == "PlayerCollider") {
		// 下記条件の場合早期リターン
		// 1. プレイヤーが攻撃中でない
		// 2. ヒットクールタイムが終了している
		// 3. 敵がダウン中
		if (!player_->GetIsAttacking() || !hitCoolTimeTimer_.GetIsFinish() || state_->GetStateName() != "Down") { return; }

		// 無限HPでない場合のみHPを減らす
		if (!isInfiniteHP_) {
			// HPを減らす
			hp_ -= player_->GetComboManager()->GetDamage();
		}
		// クールタイムタイマー開始
		hitCoolTimeTimer_.Start(kHitCoolTime_);

		// プレイヤーに攻撃が命中したことを伝える
		player_->SetIsHit(true);

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

		// ループを切る
		enemyAnim_->isLoop_ = false;
		enemyAnim_->ChangeParameter("Enemy_Damage", true);

		

		// ダメージ効果音の再生
		Audio::GetInstance()->PlayWave(damageSound_);
	}

	// フィールドと衝突している場合
	if (collider->GetColliderName() == "Level") {
		// フィールド内に存在する
		isFieldOut_ = false;
	}
}

void Enemy::OnCollisionExit(Collider* collider)
{
	// 死亡していなければ
	if (isDead_) {
		return;
	}

	// フィールドから離れた場合
	if (collider->GetColliderName() == "Level") {
		// フィールド外に出ている
		isFieldOut_ = true;
	}
}

void Enemy::CreateParameter(const std::string& name)
{
	// パラメータの生成
	animManager_->CreateAnimationParameter(name);
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Head_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Head_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Head_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_R_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Arm_L_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Translate");
	animManager_->AddSelectAnimationKeys<float>(name, "Alpha");

}

void Enemy::ChangeState(std::unique_ptr<IEnemyState> newState)
{
	// 共通初期化を行う
	newState->PreInit(this, enemyAnim_);
	// 初期化を行う
	newState->Init();

	// 初期化した新しいステートを代入
	state_ = std::move(newState);
}

void Enemy::StartHitStop(const float hitStopTime, float timeScale)
{
	// ヒットストップ秒数が0以下の場合早期リターン
	if (hitStopTime <= 0.0f) { return; }

	// 再生中アニメーションの再生を停止
	enemyAnim_->SetAnimationSpeed(timeScale);
	// ヒットストップタイマー開始
	hitStopTimer_.Start(hitStopTime);
	// ヒットストップ中状態に
	isHitStop_ = true;
}

void Enemy::HitStopUpdate()
{
	// ヒットストップタイマー終了時
	if (hitStopTimer_.GetIsFinish()) {
		// 再生中アニメーションの再生を再開
		enemyAnim_->SetAnimationSpeed(1.0f);

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
	stunParticle_ = ParticleManager::GetInstance()->CreateNewParticle("BulletSpark", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
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
	Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("Hit", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.5f);
	hit->model_->materials_[1].tex_ = TextureManager::Load("HitEffect.png");
	hit->model_->materials_[1].enableLighting_ = false;
	hit->transform_.SetParent(&bodyTransform_);
	hit->emitterDataBuffer_->data_->count = 1;
	hit->emitterDataBuffer_->data_->frequency = 1.0f;
	hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;

	// 命中破片パーティクル再生
	Particle* hitDebris = ParticleManager::GetInstance()->CreateNewParticle("HitDebris", "./Engine/Resource/Samples/Plane", "Plane.obj", 1.0f);
	hitDebris->model_->materials_[1].tex_ = TextureManager::Load("HitDebrisEffect.png");
	hitDebris->model_->materials_[1].enableLighting_ = false;
	hitDebris->transform_.translate_ = transform_.translate_;
	hitDebris->emitterDataBuffer_->data_->count = 10;
	hitDebris->emitterDataBuffer_->data_->frequency = 3.0f;
	hitDebris->emitterDataBuffer_->data_->frequencyTime = 5.0f;
}
