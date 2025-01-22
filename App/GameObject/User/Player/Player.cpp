#include "Player.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/GameManger/GameManager.h"
#include "App/GameObject/User/Enemy/EnemyBullet.h"
#include "Engine/Resource/Texture/TextureManager.h"
#include "Engine/GameObject/GameObjectManager.h"

void Player::Init()
{
	// 入力取得
	input_ = Input::GetInstance();

	//武器用のトランスフォームの初期化
	weaponTransform_.Init();
	weaponTransform_.SetParent(&transform_);

	// コライダー用のトランスフォームの初期化
	colliderTransform_.Init();
	colliderTransform_.SetParent(&transform_);
	colliderTransform_.translate_.y += colliderHeight_;

	// 落ち影用トランスフォームの初期化
	shadowTransform_.Init();
	shadowTransform_.SetParent(&transform_);

	// メッシュの追加を行う
	AddSkiningModel("Player", &transform_, "./Resources/Player", "Player.gltf");
	AddSkiningModel("Sword", &weaponTransform_, "./Resources/Sword", "Sword.gltf");
	// 落ち影用モデル
	AddNormalModel("Shadow", &shadowTransform_, "./Resources/DropShadow", "DropShadow.obj");
	normalModels_["Shadow"]->materials_[1].color_ = shadowColor_;

	// 待機アニメーションの再生
	skiningModels_["Player"]->animationManager_.PlayAnimation("Idle", 0.0f, true);
	// 武器を左手に追従するようにする
	skiningModels_["Player"]->SetBoneParent("WeaponAnchor", &weaponTransform_);

	// 剣の鏡面反射を有効にする
	skiningModels_["Sword"]->materials_[1].environmentCoefficient_ = swordEnvironmentMapStrength_;
	// ブルームエフェクトを際立たせるためにライティング無効
	skiningModels_["Sword"]->materials_[1].enableLighting_ = false;

	// コライダーの追加
	colliderWorldPos_ = colliderTransform_.GetWorldPos();
	AddColliderSphere("PlayerCollider", &colliderWorldPos_, &colliderRadius_);

	// 攻撃用の線の追加
	SwordLine_ = std::make_unique<Line>();
	SwordLine_->Init("AttackLine", linePosition_, lineThickness_, lineLength_);
	SwordLine_->SetParent(&weaponTransform_);
	SwordLine_->AddCollider("Sword", this);
	SwordLine_->rotate_.z					= (float)std::numbers::pi;
	SwordLine_->isActive_					= false;
	SwordLine_->trailMaterial_.tex_			= TextureManager::Load("./Resources", "SwordTrail.png");
	SwordLine_->trailMaterial_.color_.w		= 0.0f;
	SwordLine_->SetIsDisplayTrail(true);
	// 一度更新する
	SwordLine_->Update();

	// 行動状態を待機状態に変更
	ChangeState(std::make_unique<Root>());

	// 帯スプライト
	AddSprite("UpperObi", {640.0f, 0.0f}, {1280.0f, 0.0f}, TextureManager::Load("./Resources", "white2x2.png"));
	AddSprite("LowerObi", {640.0f, 720.0f}, {1280.0f, 0.0f}, TextureManager::Load("./Resources", "white2x2.png"));
	// 生成したスプライトの生成
	sprites_["UpperObi"]->anchorPoint_ = { 0.5f, 0.0f };
	sprites_["UpperObi"]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	sprites_["LowerObi"]->anchorPoint_ = { 0.5f, 1.0f };
	sprites_["LowerObi"]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	// UIの追加
	for (int i = 0; i < maxHP_; i++) {
		hertUISize_[i] = {48.0f, 48.0f };
		hertUITranslate_[i] = { (hertUISize_->x) + (hertUISize_->x * i) , 32.0f };
		// ハートスプライト名の取得
		std::string hert = "Hert" + std::to_string(i);
		AddSprite(hert, hertUITranslate_[i], hertUISize_[i], TextureManager::Load("./Resources", "Hert.png"));
		sprites_[hert]->color_ = fillHertColor_;
		sprites_[hert]->isActive_ = false;
	}

	// 攻撃ボタン
	AddSprite("AButton", { 1125.0f, 125.0f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Button", "Button_A.png"));
	// 剣アイコン
	AddSprite("SwordIcon", { 1140.0f, 142.5f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Icon", "MasterSword.png"));
	// Bボタン
	AddSprite("BButton", { 1185.0f, 75.0f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Button", "Button_B.png"));

	// ボタン関連UI非表示
	sprites_["AButton"]->isActive_ = false;
	sprites_["SwordIcon"]->isActive_ = false;
	sprites_["BButton"]->isActive_ = false;

	// コンボマネージャーの初期化
	comboManager_.Init(this);
	// コンボマネージャーに条件を追加する
	comboManager_.AddCondition("IsHit", &isHit_); // 攻撃の命中フラグ

	// 効果音読み込み
	SwingSword_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/SwingSword.mp3");
	RotateSlash_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/RotateSlash.mp3");
}

void Player::Update()
{
	#ifdef _DEBUG // デバッグ時のみ実行

	// ImGuiの表示
	comboManager_.DisplayImGui();

	#endif // _DEBUG // デバッグ時のみ実行

	// 落ち影更新
	ShadowUpdate();

	//コライダーのワールド座標更新
	colliderWorldPos_ = colliderTransform_.GetWorldPos();

	// ゲーム開始状態のとき
	if (gameManager_->GetIsGameStart() && !isSetUp_) {
		// メインカメラを使用
		followCamera_->UseThisCamera();

		// 体力UIの表示
		for (int i = 0; i < maxHP_; i++) {
			std::string hert = "Hert" + std::to_string(i);
			sprites_[hert]->isActive_ = true;
		}
		// ボタン関連UIの表示
		sprites_["AButton"]->isActive_		= true;
		sprites_["SwordIcon"]->isActive_	= true;
		sprites_["BButton"]->isActive_		= true;

		// 行動可能状態に
		canAction_ = true;

		// セットアップした状態に
		isSetUp_ = true;
	}

	// 敵が死亡している状態であれば
	if (enemy_->GetIsDead()) {
		// 行動出来ない状態に
		canAction_ = false;
	}

	// 死亡演出中は
	if (enemy_->GetIsDeadStaging()) {
		// 帯の表示
		sprites_["UpperObi"]->scale_.y = KLib::Lerp<float>(sprites_["UpperObi"]->scale_.y, maxObiSizeY_, obiCorrectSpeed_);
		sprites_["LowerObi"]->scale_.y = KLib::Lerp<float>(sprites_["LowerObi"]->scale_.y, maxObiSizeY_, obiCorrectSpeed_);
	}

	// 軌跡関連の更新
	TrailUpdate();

	// 行動可能状態でない場合早期リターン
	if (!canAction_) {
		// 強制待機状態に
		if (GetStateName() != "Root") {
			ChangeState(std::make_unique<Root>());
		}
		return;
	}

	// ヒットストップ中であれば更新関数呼び出しからの早期リターン
	if (isHitStop_) { 
		HitStopUpdate();
		return; 
	}

	// 現在の行動状態の更新を行う
	state_->Update();

	// 回転補正の処理
	CorrectDirectionUpdate();

	// 追従カメラがセットされている場合
	if (followCamera_ != nullptr) {
		// Z注目有効時かつ敵が死亡していない場合
		if (followCamera_->GetEnableZForcus()) {
			// ロックオン中は帯を表示
			sprites_["UpperObi"]->scale_.y = KLib::Lerp<float>(sprites_["UpperObi"]->scale_.y, maxObiSizeY_, obiCorrectSpeed_);
			sprites_["LowerObi"]->scale_.y = KLib::Lerp<float>(sprites_["LowerObi"]->scale_.y, maxObiSizeY_, obiCorrectSpeed_);
		}
		else {
			// ロックオンをしていない場合帯を非表示
			sprites_["UpperObi"]->scale_.y = KLib::Lerp<float>(sprites_["UpperObi"]->scale_.y, 0.0f, obiCorrectSpeed_);
			sprites_["LowerObi"]->scale_.y = KLib::Lerp<float>(sprites_["LowerObi"]->scale_.y, 0.0f, obiCorrectSpeed_);
		}
	}

	// 攻撃関係更新
	AttackUpdate();

	// 命中フラグのリセット
	isHit_ = false;

	//コライダーのワールド座標更新
	colliderWorldPos_ = colliderTransform_.GetWorldPos();

	// ヒットクールタイム更新
	hitCoolTimeTimer_.Update();
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();

	ImGui::DragInt("HP", &hp_);

	// 行動状態のImGuiを表示
	state_->DisplayImGui();

	// 線のImGui表示
	SwordLine_->DisplayImGui();

	// 影のa値の調整
	ImGui::DragFloat("ShadowAlpha", &normalModels_["Shadow"]->materials_[0].color_.w, 0.001f);

	// 基底クラスのImGuiを表示する
	IObject::DisplayImGui();
}

void Player::OnCollisionEnter(Collider* target, [[maybe_unused]] Collider* source)
{
	// プレイヤーの剣と衝突していたら
	if (target->GetColliderName() == "Bullet" && GetStateName() == "Attack") {
		// 敵弾取得
		EnemyBullet* bullet = GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet");

		if (bullet->GetIsReturn()) { return; }

		// パリィ演出を開始
		followCamera_->StartBlur(parryBlurStartTime_, parryBlurEndTime_, parryBlurStrength_);
	}
}

void Player::OnCollision(Collider* target, [[maybe_unused]] Collider* source)
{
	// 敵とプレイヤーのコライダーが衝突した時
	if (target->GetColliderName() == "Boss" && source->GetColliderName() == "PlayerCollider") {
		// 敵を取得
		Enemy* enemy = GameObjectManager::GetInstance()->GetGameObject<Enemy>("Enemy");
		// 敵の衝突判定半径を取得
		float radius = enemy->GetColliderRadius();

		// 敵からプレイヤーの座標取得
		Vector3 sub = Vector3::Normalize(colliderTransform_.GetWorldPos() - enemy->GetColliderPos());

		transform_.translate_ = enemy->GetColliderPos() + (sub * (radius + colliderRadius_));
		transform_.translate_.y = 0.0f;
	}
}

void Player::ChangeState(std::unique_ptr<IState> newState)
{
	// 共通初期化を行う
	newState->PreInit(this);
	// 初期化を行う
	newState->Init();

	// 初期化した新しいステートを代入
	state_ = std::move(newState);
}

void Player::StartHitStop(const float hitStopTime, const float timeScale)
{
	// ヒットストップ秒数が0以下の場合早期リターン
	if (hitStopTime <= 0.0f) { return; }

	// プレイヤーアニメーションの再生速度を指定
	skiningModels_["Player"]->animationManager_.SetAnimationSpeed(timeScale);
	// ヒットストップタイマー開始
	hitStopTimer_.Start(hitStopTime);
	// ヒットストップ中状態に
	isHitStop_ = true;

	// ゲーム速度が0の場合更新を停止
	if (timeScale > 0.0f) { return; }

	// 軌跡座標の更新を停止
	SwordLine_->SetIsUpdateTrail(false);
}

void Player::HitStopUpdate()
{
	// ヒットストップタイマー終了時
	if (hitStopTimer_.GetIsFinish()) {
		// プレイヤーアニメーションの再生速度等速へリセット
		skiningModels_["Player"]->animationManager_.SetAnimationSpeed(1.0f);
		// 軌跡座標の更新を再開
		SwordLine_->SetIsUpdateTrail(true);

		// ヒットストップ状態終了
		isHitStop_ = false;
	}

	// ヒットストップタイマーの更新
	hitStopTimer_.Update();
}

void Player::HitDamage(const Vector3& translate)
{
	// ヒットクールタイムが終了していれば
	if (hitCoolTimeTimer_.GetIsFinish()) {
		// HPを減らす
		hp_--;

		// スプライト配列探査用
		std::string hert = "Hert" + std::to_string(hp_);
		// スプライトの色を変更
		sprites_[hert]->color_ = loseHertColor_;

		// 弾の座標からプレイヤーの方向ベクトルを求める
		Vector3 sub = translate - transform_.translate_;
		// 角度を求める
		transform_.rotate_.y = std::atan2(sub.x, sub.z);

		// ダメージステートに移行
		ChangeState(std::make_unique<Damage>());

		// ロックオン無効
		followCamera_->GetLockOn()->DisableLockOn();

		// 命中クールタイムリセット
		hitCoolTimeTimer_.Start(kHitCoolTime_);
	}
}

void Player::TrailUpdate()
{
	// 軌跡a値
	float& trailAlpha = SwordLine_->trailMaterial_.color_.w;
	// 攻撃中は軌跡を表示させる
	if (isAttacking_) {
		trailAlpha = KLib::Lerp(trailAlpha, maxTrailAlpha_, startAppearTrailCorrectSpeed_);
	}
	else {
		// 攻撃中でない場合は軌跡を徐々に消す
		if (trailAlpha <= trailAlphaThresold_) {
			trailAlpha = minTrailAlpha_;
		}
		else {
			trailAlpha = KLib::Lerp(trailAlpha, minTrailAlpha_, endAppearTrailCorrectSpeed_);
		}
	}

	// 線の更新
	SwordLine_->Update();
}

void Player::AttackUpdate()
{
	// 下記条件の場合早期リターン
	// 1. プレイヤーがダメージを受けている状態
	// 2. 攻撃中状態
	// 3. 攻撃可能状態でない
	// 4. 攻撃中である
	if (state_->GetStateName() == "Damage" || 
		state_->GetStateName() == "Attack" || 
		!canAttack_ || 
		isAttacking_) { return; }

	// Aボタンを押すと攻撃する
	if (input_->InspectButton(XINPUT_GAMEPAD_A, TRIGGER)) {
		// 敵との距離を計測
		float toEnemyDistance = std::abs(Vector3::Length(enemy_->transform_.translate_ - transform_.translate_));
		// 敵との距離が補正閾値内だった場合
		if (toEnemyDistance <= correctDirectionDistance_) {
			// 回転補正を行う
			isCorrectingToEnemy_ = true;
		}

		// 行動を変更
		ChangeState(std::make_unique<Attack>());

		// Z注目有効時
		if (followCamera_->GetEnableZForcus()) {
			comboManager_.ChangeCombo("LockOn");
		}
		else {
			comboManager_.ChangeCombo("Normal");
		}

		// デバッグ有効時には表示中コンボへ切り替える
		if (comboManager_.GetEnableComboDebug()) {
			comboManager_.ChangeCombo(comboManager_.GetImGuiDisplayName());
		}
	}
}

void Player::CorrectDirectionUpdate()
{
	// 敵に対して補正をかけない状態であれば
	if (!isCorrectingToEnemy_) { 
		// ロックオン中でない場合早期リターン
		if (!followCamera_->GetLockOn()->GetIsLockOn()) { return; }

		// ロックオン対象の座標
		Vector3 targetPos = followCamera_->GetLockOn()->target_->transform_.translate_;
		// 追従対象からロックオン対象への差分ベクトル
		Vector3 sub = targetPos - transform_.translate_;
		// 方向ベクトルを元にプレイヤーがいる角度を求める
		targetAngle_ = std::atan2(sub.x, sub.z);
		// 身体を回転させる
		transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngle_, angleCorrectSpeed_);

		// この時点で終了
		return; 
	}

	// 敵がいる方向ベクトルを求める
	enemyDirection_ = enemy_->transform_.translate_ - transform_.translate_;
	// 敵がいる方向を求める
	targetAngle_ = std::atan2(enemyDirection_.x, enemyDirection_.z);
	// 閾値を超えるまで補正を続ける
	if (std::abs(transform_.rotate_.y - targetAngle_) > angleCorrectThreshold_){
		// 身体を回転させる
		transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngle_, toEnemyCorrectSpeed_);
	}
	else {
		// 補正終了
		isCorrectingToEnemy_ = false;
	}
}

void Player::ShadowUpdate()
{
	// プレイヤーのHipボーンの座標取得
	Vector3 hipBonePosition = skiningModels_["Player"]->GetBonePosition("Hip");

	// Hipボーンの座標を元に影の大きさを変更
	float shadowScale = KLib::Lerp<float>(maxShadowScale, minShadowScale, hipBonePosition.y, maxHipBoneHeight_);
	shadowTransform_.scale_ = { shadowScale, 1.0f, shadowScale };

	// プレイヤーのHipボーンと同じ座標に
	shadowTransform_.translate_ = hipBonePosition;
	// 高さは自動調整
	shadowTransform_.translate_.y = -(transform_.GetWorldPos().y) + shadowHeight_;
}
