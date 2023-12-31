#include "ThirdPersonCamera.h"

void ThirdPersonCamera::Init()
{
	// 基底クラスの初期化を呼び出す
	Camera::Init();

	// 入力取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// 追従対象の目標角度
	targetAngle_ = 0.0f;
	// カメラの追従遅延量
	trackingDelay_ = 0.35f;

}

void ThirdPersonCamera::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// 旋回速度
	const float speed = 0.05f;

	// スティックの横移動に対してカメラを旋回
	targetAngle_ += (float)joyState_.Gamepad.sThumbRX / SHRT_MAX * speed;
	if (targetAngle_ >= (float)std::numbers::pi * 2.0f || (float)-std::numbers::pi * 2.0f >= targetAngle_)
		targetAngle_ = 0.0f;

	if (joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_THUMB) {
		targetAngle_ = target_->rotate_.y;
	}

	transform_.rotate_.y = Math::LerpShortAngle(transform_.rotate_.y, targetAngle_, 0.25f);

	UpdateTarget();
}

void ThirdPersonCamera::Reset()
{
	targetAngle_ = transform_.translate_.y;

	// 追従対象が存在すれば
	if (target_) {
		// 追従座標と角度の初期化
		interTarget_ = target_->translate_;
		transform_.rotate_.y = target_->rotate_.y;
		targetAngle_ = target_->rotate_.y;
	}

	Vector3 offset = CalcOffset();
	transform_.translate_ = interTarget_ + offset;
}

void ThirdPersonCamera::SetTarget(const WorldTransform* target)
{
	target_ = target;
	Reset();
}

void ThirdPersonCamera::UpdateTarget()
{
	if (target_)
		interTarget_ = Math::Linear(trackingDelay_, interTarget_, target_->translate_);
	Vector3 offset = CalcOffset();
	transform_.translate_ = interTarget_ + offset;

	// 基底クラスの更新を呼び出す
	Camera::Update();
}

Vector3 ThirdPersonCamera::CalcOffset() const
{
	// 追従対象からカメラまでのオフセットを設定
	Vector3 offset = { 0.0f, 2.0f, -10.0f };

	// カメラの角度から回転行列を生成
	Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(transform_.rotate_);

	// オフセットをカメラの回転に合わせて回転させる
	offset = Math::Transform(offset, rotateMat);

	// 計算したオフセットを返す
	return offset;
}
