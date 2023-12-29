#include "FollowCamera.h"

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

	if (joyState_.Gamepad.bLeftTrigger >= 10.0f) {
		// 目標角度をプレイヤーの真後ろに
		targetAngle_ = target_->rotate_.y;
		// Z注目を有効に
		enableZForcus_ = true;
		// 角度補正速度を設定
		correctionSpeed_ = zForcusCorrectionSpeed_;
	}
	else {
		// Z注目をしていないに
		enableZForcus_ = false;
		// 角度補正速度を設定
		correctionSpeed_ = normalCorrectionSpeed_;
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
