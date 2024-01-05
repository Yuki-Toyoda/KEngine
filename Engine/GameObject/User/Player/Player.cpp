#include "Player.h"
#include "../LockOn/LockOn.h"
#include "../FollowCamera/FollowCamera.h"
#include "../Enemy/Enemy.h"
#include "../../../Resource/Texture/TextureManager.h"

void Player::Init()
{
	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// アニメーションマネージャの取得
	animManager_ = AnimationManager::GetInstance();

	// 全てのトランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_, 0b111);
	headTransform_.Init();
	headTransform_.SetParent(&bodyTransform_, 0b111);
	headTransform_.translate_ = { 0.0f, 1.35f, 0.0f };
	armTransform_R_.Init();
	armTransform_R_.SetParent(&bodyTransform_, 0b111);
	armTransform_R_.translate_ = { 0.0f, 1.3f, 0.0f };
	armTransform_L_.Init();
	armTransform_L_.SetParent(&bodyTransform_, 0b111);
	armTransform_L_.translate_ = { 0.0f, 1.3f, 0.0f };


	// アニメーションパラメータの追加

	// 待機状態
	animManager_->CreateAnimationParameter("Player_Idle");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Head_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Head_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Head_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Arm_R_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Arm_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Arm_R_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Arm_L_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Arm_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Idle", "Arm_L_Translate");

	// 待機状態
	animManager_->CreateAnimationParameter("Player_Run");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Head_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Head_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Head_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Arm_R_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Arm_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Arm_R_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Arm_L_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Arm_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_Run", "Arm_L_Translate");

	// 横切り
	animManager_->CreateAnimationParameter("Player_HorizontalSlash");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Body_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Body_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Body_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Head_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Head_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Head_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Arm_R_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Arm_R_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Arm_R_Translate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Arm_L_Scale");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Arm_L_Rotate");
	animManager_->AddSelectAnimationKeys<Vector3>("Player_HorizontalSlash", "Arm_L_Translate");


	// アニメーションの作成
	playerAnim_ = AnimationManager::GetInstance()->CreateAnimation("PlayerAnimation", "Player_Idle");
	playerAnim_->AddAnimationKeys<Vector3>("Body_Scale", &bodyTransform_.scale_);
	playerAnim_->AddAnimationKeys<Vector3>("Body_Rotate", &bodyTransform_.rotate_);
	playerAnim_->AddAnimationKeys<Vector3>("Body_Translate", &bodyTransform_.translate_);
	playerAnim_->AddAnimationKeys<Vector3>("Head_Scale", &headTransform_.scale_);
	playerAnim_->AddAnimationKeys<Vector3>("Head_Rotate", &headTransform_.rotate_);
	playerAnim_->AddAnimationKeys<Vector3>("Head_Translate", &headTransform_.translate_);
	playerAnim_->AddAnimationKeys<Vector3>("Arm_R_Scale", &armTransform_R_.scale_);
	playerAnim_->AddAnimationKeys<Vector3>("Arm_R_Rotate", &armTransform_R_.rotate_);
	playerAnim_->AddAnimationKeys<Vector3>("Arm_R_Translate", &armTransform_R_.translate_);
	playerAnim_->AddAnimationKeys<Vector3>("Arm_L_Scale", &armTransform_L_.scale_);
	playerAnim_->AddAnimationKeys<Vector3>("Arm_L_Rotate", &armTransform_L_.rotate_);
	playerAnim_->AddAnimationKeys<Vector3>("Arm_L_Translate", &armTransform_L_.translate_);

	// ループ状態にする
	playerAnim_->isLoop_ = true;
	// この状態で再生
	playerAnim_->Play();

	// メッシュの追加を行う
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Body.obj");
	AddMesh(&headTransform_, color_, "./Resources/Player", "Head.obj");
	AddMesh(&armTransform_R_, color_, "./Resources/Player", "Arm_R.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Player", "Arm_L.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Sword", "Sword.obj");
	AddMesh(&armTransform_R_, color_, "./Resources/Shield", "Shiled.obj");

	attackLine_ = std::make_unique<Line>();
	attackLine_->Init("AttackLine", {-1000.0f, 100.0f, 0.0f}, {0.35f, 0.35f}, 1.0f, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
	attackLine_->SetParent(&armTransform_L_);
	attackLine_->AddCollider("Sword", this);
	attackLine_->rotate_.x = (float)std::numbers::pi / 2.0f;

	// 行動状態を待機状態に変更
	ChangeState(std::make_unique<Root>());

}

void Player::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// 現在の行動状態の更新を行う
	state_->Update();

	if (followCamera_ != nullptr) {
		if (!followCamera_->GetEnableZForcus()) {
			followCamera_->SetTargetAngle(followCamera_->transform_.rotate_.y);
		}

		if (followCamera_->GetLockOn()->GetIsLockOn()) {
			// ロックオン対象の座標
			Vector3 targetPos = followCamera_->GetLockOn()->target_->transform_.translate_;
			// 追従対象からロックオン対象への差分ベクトル
			Vector3 sub = targetPos - transform_.translate_;
			// 方向ベクトルを元にプレイヤーがいる角度を求める
			targetAngle_ = std::atan2(sub.x, sub.z);

			// 身体を回転させる
			transform_.rotate_.y = Math::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.1f);
		}
	}

	// 攻撃可能か
	if (canAttack_ && !isAttacking_) {
		// Aボタンを押すと攻撃する
		if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
			!(preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			// 行動を変更
			ChangeState(std::make_unique<Attack>());
		}
	}

	// 線の更新
	attackLine_->Update();
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
	bodyTransform_.DisplayImGuiWithTreeNode("BodyTransform");
	headTransform_.DisplayImGuiWithTreeNode("HeadTransform");
	armTransform_R_.DisplayImGuiWithTreeNode("Arm_R_Transform");
	armTransform_L_.DisplayImGuiWithTreeNode("Arm_L_BodyTransform");

	// プレイヤーのアニメーションImGui
	playerAnim_->DisplayImGui();
	// アニメーションの読み込みパラメータ変更
	if (ImGui::TreeNode("ChangeReadParameter")) {
		if (ImGui::Button("Idle")) {
			playerAnim_->ChangeParameter("Player_Idle", true);
		}
		if (ImGui::Button("Run")) {
			playerAnim_->ChangeParameter("Player_Run", true);
		}
		if (ImGui::Button("HorizontalSlash")) {
			playerAnim_->ChangeParameter("Player_HorizontalSlash", true);
		}
		ImGui::TreePop();
	}

	// 行動状態のImGuiを表示
	state_->DisplayImGui();

	// 線のImGui表示
	attackLine_->DisplayImGui();
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
