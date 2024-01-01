#include "Player.h"
#include "../LockOn/LockOn.h"
#include "../FollowCamera/FollowCamera.h"
#include "../Enemy/Enemy.h"

void Player::Init()
{
	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// 全てのトランスフォームの初期化
	bodyTransform_.Init();
	bodyTransform_.SetParent(&transform_, 0b011);
	headTransform_.Init();
	headTransform_.SetParent(&bodyTransform_, 0b011);
	headTransform_.translate_ = { 0.0f, 1.35f, 0.0f };
	armTransform_R_.Init();
	armTransform_R_.SetParent(&bodyTransform_, 0b011);
	armTransform_R_.translate_ = { 0.0f, 1.3f, 0.0f };
	armTransform_L_.Init();
	armTransform_L_.SetParent(&bodyTransform_, 0b011);
	armTransform_L_.translate_ = { 0.0f, 1.3f, 0.0f };

	// メッシュの追加を行う
	AddMesh(&bodyTransform_, color_, "./Resources/Player", "Body.obj");
	AddMesh(&headTransform_, color_, "./Resources/Player", "Head.obj");
	AddMesh(&armTransform_R_, color_, "./Resources/Player", "Arm_R.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Player", "Arm_L.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Sword", "Sword.obj");

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
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
	bodyTransform_.DisplayImGuiWithTreeNode("BodyTransform");
	headTransform_.DisplayImGuiWithTreeNode("HeadTransform");
	armTransform_R_.DisplayImGuiWithTreeNode("Arm_R_Transform");
	armTransform_L_.DisplayImGuiWithTreeNode("Arm_L_BodyTransform");

	// 行動状態のImGuiを表示
	state_->DisplayImGui();
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
