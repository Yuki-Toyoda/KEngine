#include "FollowCamera.h"
#include "../LockOn/LockOn.h"
#include "../Enemy/Enemy.h"

void FollowCamera::Init()
{
	// 基底クラスの初期化を行う
	Camera::Init();
}

void FollowCamera::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// 旋回速度
	//const float speed = 0.05f;

	// スティックの横移動に対してカメラを旋回
	/*targetAngle_ += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * speed;
	if (targetAngle_ >= (float)std::numbers::pi * 2.0f || (float)-std::numbers::pi * 2.0f >= targetAngle_)
		targetAngle_ = 0.0f;*/

	if (joyState_.Gamepad.bLeftTrigger >= 35.0f) {
		// ロックオンを有効
		if (!lockOn_->EnableLockOn()) {
			// 目標角度をプレイヤーの真後ろに
			targetAngle_ = target_->rotate_.y;
			// 角度補正速度を設定
			correctionSpeed_ = zForcusCorrectionSpeed_;
		}
		else {
			// ロックオン対象の座標
			Vector3 targetPos = lockOn_->target_->transform_.translate_;
			// 追従対象からロックオン対象への差分ベクトル
			Vector3 sub = targetPos - target_->translate_;
			//sub = Math::Normalize(sub);

			// 方向ベクトルを元にプレイヤーがいる角度を求める
			//targetAngle_ = Math::Angle({ 0.0f, 0.0f, 1.0f }, sub);
			targetAngle_ = std::atan2(sub.x, sub.z);

			/*if (targetAngle_ >= (float)std::numbers::pi * 2.0f) {
				targetAngle_ -= (float)std::numbers::pi * 2.0f;
			}
			if (targetAngle_ <= -(float)std::numbers::pi * 2.0f) {
				targetAngle_ += (float)std::numbers::pi * 2.0f;
			}*/

			// 角度補正速度を設定
			correctionSpeed_ = zEnemyForcusCorrectionSpeed_;
		}

		// Z注目を有効に
		enableZForcus_ = true;
	}
	else {
		// Z注目をしていないに
		enableZForcus_ = false;
		// 角度補正速度を設定
		correctionSpeed_ = normalCorrectionSpeed_;

		// ロックオンを無効
		lockOn_->DisableLockOn();
	}

	// 回転させる
	transform_.rotate_.y = Math::LerpShortAngle(transform_.rotate_.y, targetAngle_, correctionSpeed_);
	

	// 追従対象の更新
	UpdateTarget();
}

void FollowCamera::DisplayImGui()
{
	// 基底クラスのImGuiを表示
	Camera::DisplayImGui();

	// 目標角度表示
	ImGui::DragFloat("TargetAngle", &targetAngle_);
}

void FollowCamera::UpdateTarget()
{
	// 追従対象が存在するときのみ
	if (target_) {
		// 遷移後の座標を求める
		interTarget_ = Math::Linear(trackingDelay_, interTarget_, target_->translate_);
	}

	// オフセットの取得
	Vector3 offset = CalcOffset();
	// カメラの座標をオフセットを元に変更
	transform_.translate_ = interTarget_ + offset;

	// 基底クラスの更新を行う
	Camera::Update();
}

Vector3 FollowCamera::CalcOffset() const
{
	// 追従対象からカメラまでのオフセットを設定
	Vector3 offset = offset_;

	// カメラの角度から回転行列を生成
	Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);

	// オフセットをカメラの回転に合わせて回転させる
	offset = Math::Transform(offset, rotateMat);

	// 計算したオフセットを返す
	return offset;
}
