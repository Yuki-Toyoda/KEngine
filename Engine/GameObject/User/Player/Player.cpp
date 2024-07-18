#include "Player.h"
#include "../LockOn/LockOn.h"
#include "../FollowCamera/FollowCamera.h"
#include "../Enemy/Enemy.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../../GameObjectManager.h"
#include "../../Core/Camera.h"
#include "../Enemy/Enemy.h"

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
	skiningModels_[0]->animationManager_.PlayAnimation("00_Idle", true);
	// 武器を左手に追従するようにする
	skiningModels_[0]->SetBoneParent("WeaponAnchor", &weaponTransform_);

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

	// UIの追加
	for (int i = 0; i < 6; i++) {
		hertUISize_[i] = {48.0f, 48.0f };
		hertUITranslate_[i] = { (hertUISize_->x) + (hertUISize_->x * i) , 32.0f };
		AddSprite("Hert", hertUITranslate_[i], hertUISize_[i], TextureManager::Load("./Resources", "Hert.png"));
		sprites_[i]->color_ = { 1.0f, 0.0f, 0.15f, 1.0f };
		sprites_[i]->isActive_ = false;
	}

	// UIの追加
	AddSprite("UI", {0.0f, 0.0f}, {1280.0f ,720.0f}, TextureManager::Load("./Resources", "UI.png"));
	sprites_[6]->isActive_ = false;

	// タイトル画面ボタン用
	AddSprite("Button", {640.0f, 450.0f}, {512.0f ,128.0f}, TextureManager::Load("Start_CR.png"));
	// フェードスプライトの色設定
	sprites_[7]->anchorPoint_ = { .5f, .5f };
	
	// フェード演出用
	AddSprite("Fade", {0.0f, 0.0f}, {1280.0f ,720.0f}, TextureManager::Load("white2x2.png"));
	// フェードスプライトの色設定
	sprites_[8]->color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

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
				for (int i = 0; i < 6; i++) {
					sprites_[i]->isActive_ = true;
				}
				sprites_[6]->isActive_ = true;

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
				sprites_[7]->isActive_ = false;

				// タイトルアニメーション再生
				titleAnim_->ChangeParameter("Title_Start", true);
				// ループ無効
				titleAnim_->isLoop_ = false;
			}
		}

		if (sprites_[8]->color_.w <= 0.0f) {
			
		}
		else {
			// フェード演出用スプライトの色を徐々に変更
			sprites_[8]->color_.w -= 0.01f;
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
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
	headTransform_.DisplayImGuiWithTreeNode("HeadTransform");
	weaponTransform_.DisplayImGuiWithTreeNode("Weapon");

	ImGui::DragInt("HP", &hp_);

	// タイトルアニメーションのImGuiを表示
	titleAnim_->DisplayImGui();

	// 行動状態のImGuiを表示
	state_->DisplayImGui();

	// 線のImGui表示
	attackLine_->DisplayImGui();

	colliderTransform_.DisplayImGui("collider");
	Vector3 world = colliderTransform_.GetWorldPos();
	ImGui::DragFloat3("colliderWorldPos", &world.x);
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
		sprites_[hp_]->color_ = { 0.15f, 0.15f, 0.15f, 1.0f };

		// 弾の座標からプレイヤーの方向ベクトルを求める
		Vector3 sub = translate - transform_.translate_;
		// 角度を求める
		transform_.rotate_.y = std::atan2(sub.x, sub.z);

		ChangeState(std::make_unique<Damage>());

		lockOn_->DisableLockOn();

		// 命中クールタイムリセット
		hitCoolTimeTimer_.Start(kHitCoolTime_);
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
	// 敵と衝突時ヒットストップを行う
	if (collider->GetGameObject()->GetObjectTag() == IObject::TagEnemy) {
		//HitStop();
	}
}

void Player::CreateTitleCameraParameter(const std::string& name)
{
	// パラメータの生成
	animManager_->CreateAnimationParameter(name);
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>(name, "Camera_Translate");
}
