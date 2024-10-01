#include "Player.h"
#include "../LockOn/LockOn.h"
#include "../FollowCamera/FollowCamera.h"
#include "../Enemy/Enemy.h"
#include "../Enemy/EnemyBullet.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../../GameObjectManager.h"
#include "../../Core/Camera.h"

void Player::Init()
{
	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// アニメーションマネージャの取得
	animManager_ = AnimationManager::GetInstance();

	//武器用のトランスフォームの初期化
	weaponTransform_.Init();
	weaponTransform_.SetParent(&transform_);

	// コライダー用のトランスフォームの初期化
	colliderTransform_.Init();
	colliderTransform_.SetParent(&transform_);
	colliderTransform_.translate_.y += 1.0f;

	/// タイトル演出用パラメータを生成
	CreateTitleCameraParameter("Title_Idle");
	CreateTitleCameraParameter("Title_Start");

	// タイトル演出用カメラを生成
	titleCamera_ = GameObjectManager::GetInstance()->CreateInstance<Camera>("TitleCamera", IObject::TagCamera);
	titleCamera_->transform_.translate_ = { -1.5f, 0.92f, -3.5f };
	titleCamera_->transform_.rotate_ = { -0.15f, 0.325f, 0.0f };
	titleCamera_->ppProcessor_.bloom_.threshold_ = 0.25f;

	// アニメーション生成
	titleAnim_ = AnimationManager::GetInstance()->CreateAnimation("TitleAnim", "Title_Idle");
	titleAnim_->AddAnimationKeys<Vector3>("Camera_Rotate", &titleCamera_->transform_.rotate_);
	titleAnim_->AddAnimationKeys<Vector3>("Camera_Translate", &titleCamera_->transform_.translate_);

	// ループ状態にする
	titleAnim_->isLoop_ = true;
	// この状態で再生
	titleAnim_->Play();

	// メッシュの追加を行う
	AddSkiningModel(&transform_, "./Resources/Player", "Player.gltf");
	AddSkiningModel(&weaponTransform_, "./Resources/Sword", "Sword.gltf");

	// 待機アニメーションの再生
	skiningModels_[0]->animationManager_.PlayAnimation("00_Idle", 0.0f, true);
	// 武器を左手に追従するようにする
	skiningModels_[0]->SetBoneParent("WeaponAnchor", &weaponTransform_);

	// 剣の鏡面反射を有効にする
	skiningModels_[1]->materials_[1].environmentCoefficient_ = 0.85f;
	// ブルームエフェクトを際立たせるためにライティング無効
	skiningModels_[1]->materials_[1].enableLighting_ = false;

	// コライダーの追加
	colliderWorldPos_ = colliderTransform_.GetWorldPos();
	AddColliderSphere("PlayerCollider", &colliderWorldPos_, &colliderRadius_);

	// 攻撃用の線の追加
	attackLine_ = std::make_unique<Line>();
	attackLine_->Init("AttackLine", {0.0f, 0.0f, 0.0f}, {0.35f, 0.35f}, 1.0f);
	attackLine_->SetParent(&weaponTransform_);
	attackLine_->AddCollider("Sword", this);
	attackLine_->rotate_.z = (float)std::numbers::pi;
	attackLine_->isActive_ = false;
	attackLine_->Update();

	// 行動状態を待機状態に変更
	ChangeState(std::make_unique<Root>());

	// 帯スプライト
	AddSprite("UpperObi", {640.0f, 0.0f}, {1280.0f, 0.0f}, TextureManager::Load("./Resources", "white2x2.png"));
	AddSprite("LowerObi", {640.0f, 720.0f}, {1280.0f, 0.0f}, TextureManager::Load("./Resources", "white2x2.png"));
	
	sprites_[0]->anchorPoint_ = { 0.5f, 0.0f };
	sprites_[0]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };
	sprites_[1]->anchorPoint_ = { 0.5f, 1.0f };
	sprites_[1]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	// UIの追加
	for (int i = 2; i < 8; i++) {
		hertUISize_[i - 2] = {48.0f, 48.0f };
		hertUITranslate_[i - 2] = { (hertUISize_->x) + (hertUISize_->x * (i - 2)) , 32.0f };
		AddSprite("Hert", hertUITranslate_[i - 2], hertUISize_[i - 2], TextureManager::Load("./Resources", "Hert.png"));
		sprites_[i]->color_ = { 1.0f, 0.0f, 0.15f, 1.0f };
		sprites_[i]->isActive_ = false;
	}

	// タイトル画面ボタン用
	AddSprite("StartButton", { 640.0f, 450.0f }, { 512.0f ,128.0f }, TextureManager::Load("Start_CR.png"));
	// アンカーポイントの設定
	sprites_[8]->anchorPoint_ = { .5f, .5f };

	// 攻撃ボタン
	AddSprite("AButton", { 1125.0f, 125.0f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Button", "Button_A.png"));
	// 剣アイコン
	AddSprite("SwordIcon", { 1140.0f, 142.5f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Icon", "MasterSword.png"));
	// Bボタン
	AddSprite("BButton", { 1185.0f, 75.0f }, { 48.0f ,48.0f }, TextureManager::Load("./Resources/UI/Button", "Button_B.png"));
	// フェード演出用スプライト
	AddSprite("BButton", { 
		(float)KEngine::Config::Window::KWindowWidth / 2.0f, 
		(float)KEngine::Config::Window::KWindowHeight / 2.0f, }, 
		{ (float)KEngine::Config::Window::KWindowWidth,
		(float)KEngine::Config::Window::KWindowHeight }, 
		TextureManager::Load("white2x2.png"));
	// アンカーポイントの設定
	sprites_[12]->anchorPoint_ = { .5f, .5f };
	// 色、透明度設定
	sprites_[12]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ボタン関連UI非表示
	sprites_[9]->isActive_ = false;
	sprites_[10]->isActive_ = false;
	sprites_[11]->isActive_ = false;

	// 効果音読み込み
	SwingSword_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/SwingSword.mp3");
	RotateSlash_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/RotateSlash.mp3");
}

void Player::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// フェードインされているかつゲームスタートをしていない場合
	if (!isGameStart_) {

		if (!isFade_) {
			// タイトルカメラを使用する
			titleCamera_->UseThisCamera();
		}
		else {
			// タイトルアニメーション終了時
			if (titleAnim_->isEnd_) {
				// メインカメラを使用
				followCamera_->UseThisCamera();

				// UIの表示
				for (int i = 2; i < 8; i++) {
					sprites_[i]->isActive_ = true;
				}

				// ボタン関連UI表示
				sprites_[9]->isActive_ = true;
				sprites_[10]->isActive_ = true;
				sprites_[11]->isActive_ = true;

				// ゲームスタート
				isGameStart_ = true;
				// 敵にも開始したことを伝える
				enemy_->SetIsGameStart(true);
			}
		}

		if (!isFade_) {
			// Aボタンを押すと
			if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {

				// 開始演出を開始
				isFade_ = true;

				// ボタンUI非表示
				sprites_[8]->isActive_ = false;

				// タイトルアニメーション再生
				titleAnim_->ChangeParameter("Title_Start", true);
				// ループ無効
				titleAnim_->isLoop_ = false;

				sprites_[12]->color_.w = 0.0f;
			}
		}

		if (sprites_[12]->color_.w <= 0.0f) {
			sprites_[12]->color_.w = 0.0f;
		}
		else {
			// フェード演出用スプライトの色を徐々に変更
			sprites_[12]->color_.w -= 0.01f;
		}
	}

	if (canMove_ && isGameStart_) {
		// 現在の行動状態の更新を行う
		state_->Update();

		if (followCamera_ != nullptr) {
			if (!followCamera_->GetEnableZForcus()) {
				followCamera_->SetTargetAngle(followCamera_->transform_.rotate_.y);
				// ロックオン中は徐々にビネット解除
				focusVignette_ = KLib::Lerp<float>(focusVignette_, 0.0f, 0.1f);
			}
			else {
				// ロックオン中は徐々にビネット
				focusVignette_ = KLib::Lerp<float>(focusVignette_, 1.0f, 0.1f);
			}

			// ビネットをかける
			followCamera_->ppProcessor_.vignette_.intensity_ = focusVignette_;

			if (followCamera_->GetLockOn()->GetIsLockOn()) {
				// ロックオン対象の座標
				Vector3 targetPos = followCamera_->GetLockOn()->target_->transform_.translate_;
				// 追従対象からロックオン対象への差分ベクトル
				Vector3 sub = targetPos - transform_.translate_;
				// 方向ベクトルを元にプレイヤーがいる角度を求める
				targetAngle_ = std::atan2(sub.x, sub.z);

				// 身体を回転させる
				transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.1f);
			}

			// ブラーの値の参照取得
			float& blurStrength = followCamera_->ppProcessor_.radialBlur_.data_.blurStrength;
			// パリィブラー有効時
			if (enableParryBlur_) {
				// ブラー強さが一定の値を超えるまで
				if (blurStrength <= 0.01f) {
					// ブラー強さを少しずつ上げる
					blurStrength = KLib::Lerp(blurStrength, 0.0115f, 0.35f);
				}
				else {
					// パリィ時のブラーを無効
					enableParryBlur_ = false;
				}
			}
			else {
				// かかっているブラーを元に戻す
				if (blurStrength > 0.0f) {
					blurStrength = KLib::Lerp(blurStrength, 0.00f, 0.05f);
				}
			}
		}

		// 攻撃可能か
		if (state_->GetStateName() != "Damage") {
			if (canAttack_ && !isAttacking_) {
				// Aボタンを押すと攻撃する
				if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
					!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
					// 行動を変更
					ChangeState(std::make_unique<Attack>());
				}
			}
		}

		// 線の更新
		attackLine_->Update();

		//コライダーのワールド座標更新
		colliderWorldPos_ = colliderTransform_.GetWorldPos();

		// ヒットクールタイム更新
		hitCoolTimeTimer_.Update();

	}
	else {

		if (!skiningModels_[0]->animationManager_.GetIsPlayingAnimation("00_Idle")) {
			skiningModels_[0]->animationManager_.PlayAnimation("00_Idle", true);
		}
	}

	// 敵弾が存在する場合帯を出現させる
	if (GameObjectManager::GetInstance()->GetGameObject<EnemyBullet>("EnemyBullet") != nullptr) {
		sprites_[0]->scale_.y = KLib::Lerp<float>(sprites_[0]->scale_.y, 75.0f, 0.1f);
		sprites_[1]->scale_.y = KLib::Lerp<float>(sprites_[0]->scale_.y, 75.0f, 0.1f);
	}
	else {
		sprites_[0]->scale_.y = KLib::Lerp<float>(sprites_[0]->scale_.y, 0.0f, 0.2f);
		sprites_[1]->scale_.y = KLib::Lerp<float>(sprites_[0]->scale_.y, 0.0f, 0.2f);
	}
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();

	ImGui::DragInt("HP", &hp_);

	// 行動状態のImGuiを表示
	state_->DisplayImGui();

	// 線のImGui表示
	attackLine_->DisplayImGui();

	sprites_[0]->DisplayImGui();
	sprites_[12]->DisplayImGui();

	// 基底クラスのImGuiを表示する
	IObject::DisplayImGui();
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

void Player::HitDamage(const Vector3& translate)
{
	// ヒットクールタイムが終了していれば
	if (hitCoolTimeTimer_.GetIsFinish()) {
		// HPを減らす
		hp_--;

		// スプライトの色を変更
		sprites_[hp_ + 2]->color_ = { 0.15f, 0.15f, 0.15f, 1.0f };

		// 弾の座標からプレイヤーの方向ベクトルを求める
		Vector3 sub = translate - transform_.translate_;
		// 角度を求める
		transform_.rotate_.y = std::atan2(sub.x, sub.z);

		ChangeState(std::make_unique<Damage>());

		lockOn_->DisableLockOn();

		// 命中クールタイムリセット
		hitCoolTimeTimer_.Start(kHitCoolTime_);

		// ダメージ時にはラジアルブラー無効
		enableParryBlur_ = false;
	}
}

void Player::HitStop()
{
	// ヒットストップを有効にする
	enableHitStop_ = true;
	// ヒットストップタイマーを開始
	hitStopTimer_.Start(kHitStopTime_);
}

void Player::OnCollisionEnter(Collider* collider)
{
	// プレイヤーの剣と衝突していたら
	if (collider->GetColliderName() == "Bullet" && !enableParryBlur_) {
		// パリィ時のブラーを有効
		enableParryBlur_ = true;
	}
}

void Player::CreateTitleCameraParameter(const std::string& name)
{
	// パラメータの生成
	animManager_->CreateAnimationParameter(name);
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Translate");
}
