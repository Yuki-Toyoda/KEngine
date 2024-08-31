#include "FollowCamera.h"
#include "../LockOn/LockOn.h"
#include "../Enemy/Enemy.h"

void FollowCamera::Init()
{
	// 基底クラスの初期化を行う
	Camera::Init();

	// オフセットなしのトランスフォームの初期化
	noOffsetTransform_.Init();

	// 球コライダー追加
	AddColliderSphere("collider", &transform_.translate_, &colliderRadius_);
}

void FollowCamera::Update()
{
	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	if (joyState_.Gamepad.bLeftTrigger >= 35.0f) {
		// ロックオンが有効になっていない場合
		if (!lockOn_->EnableLockOn()) {
			// セットアップトリガーをfalse
			isLockOnSetUp_ = false;
			
			// 目標角度をプレイヤーの真後ろに
			targetAngleY_ = target_->rotate_.y;
			// 角度補正速度を設定
			correctionSpeed_ = zForcusCorrectionSpeed_;

			// X軸の目標角度は0に
			targetAngleX_ = 0.0f;
			// 計算したのちにオフセットのリセット
			targetAngleXOffset_ = 0.0f;

			// オフセットのY軸をリセット
			offset_.y = kOffset_.y;
			offset_.z = kOffset_.z;
		}
		else { // ロックオンが有効になっている場合
			// ロックオン対象の座標
			Vector3 targetPos = lockOn_->target_->transform_.translate_;
			// 追従対象からロックオン対象への差分ベクトル
			Vector3 sub = targetPos - target_->translate_;

			// ロックオン時の座標を計算
			lockOnTranslate_ = targetPos - (sub / 2.0f);

			// 方向ベクトルを元にプレイヤーがいる角度を求める
			targetAngleY_ = std::atan2(sub.x, sub.z);

			// セットアップを行っていない場合、ここで行う
			if (!isLockOnSetUp_) {
				// 目標角度が負方向であれば
				if (targetAngleY_ < 0.0f) {
					isRightLockOn_ = false;
				}
				else {
					isRightLockOn_ = true;
				}

				// セットアップを実行済み状態に
				isLockOnSetUp_ = true;
			}

			// オフセットのY軸を0に
			offset_.y = 0.0f;
			offset_.z = -Vector3::Length(sub / 2.0f) + kOffset_.z;

			// どちらに角度を補正するかで処理を変更する
			if (isRightLockOn_) {
				kOffsetRotate_ = KLib::Lerp<float>(-0.55f, -0.15f, offset_.z, -27.0f);
			}
			else {
				kOffsetRotate_ = KLib::Lerp<float>(0.55f, 0.15f, offset_.z, -27.0f);
			}

			// 目標角度にオフセット分を加算する
			targetAngleY_ += kOffsetRotate_;
			Matrix4x4 rotateMat =
				Matrix4x4::MakeRotateY(-std::atan2(sub.x, sub.z));
			Vector3 subA = sub * rotateMat;
			targetAngleX_ = std::atan2(-subA.y, subA.z) / 3.5f;
			if (targetAngleX_ < -0.1f) {
				targetAngleX_ = -0.1f;
			}

			// 角度補正速度を設定
			correctionSpeed_ = zEnemyForcusCorrectionSpeed_;
		}

		// Z注目を有効に
		enableZForcus_ = true;
	}
	else { // Lトリガーを押していない場合
		// セットアップトリガーをfalse
		isLockOnSetUp_ = false;

		// Z注目をしていないに
		enableZForcus_ = false;
		// 角度補正速度を設定
		correctionSpeed_ = normalCorrectionSpeed_;

		// X軸の目標角度は0に
		targetAngleX_ = 0.0f;

		// オフセットのY軸をリセット
		offset_.y = kOffset_.y;
		offset_.z = kOffset_.z;
		// ロックオンを無効
		lockOn_->DisableLockOn();

	}

	// 回転させる
	transform_.rotate_.x = KLib::LerpShortAngle(transform_.rotate_.x, targetAngleX_, zEnemyForcusCorrectionSpeed_) + targetAngleXOffset_;
	transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngleY_, correctionSpeed_);

	// 計算したのちにオフセットのリセット
	targetAngleXOffset_ = 0.0f;

	// 追従対象の更新
	UpdateTarget();
}

void FollowCamera::DisplayImGui()
{
	// 基底クラスのImGuiを表示
	Camera::DisplayImGui();

	// 目標角度表示
	ImGui::DragFloat("TargetAngleX", &targetAngleX_);
	ImGui::DragFloat("TargetAngleY", &targetAngleY_);
	// 目標角度表示
	ImGui::DragFloat3("offset", &offset_.x, 0.05f);

	ImGui::DragFloat("RotateOffset", &kOffsetRotate_);
}

void FollowCamera::UpdateTarget()
{
	// 追従対象が存在するときのみ
	if (target_) {
		if (lockOn_->GetIsLockOn()) {
			// 遷移後の座標を求める
			interTarget_ = KLib::Lerp<Vector3>(interTarget_, lockOnTranslate_, trackingDelay_);
		}
		else {
			// 遷移後の座標を求める
			interTarget_ = KLib::Lerp<Vector3>(interTarget_, target_->translate_, trackingDelay_);
		}

		// オフセットなしのトランスフォームの値を設定
		noOffsetTransform_.rotate_ = transform_.rotate_;

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

		// 遷移後の座標を求める
		Vector3 interTargetA = Vector3(0.0f, 0.0f, 0.0f);
		interTargetA = KLib::Lerp<Vector3>(target_->translate_, interTargetA, trackingDelay_);
		noOffsetTransform_.translate_ = KLib::Lerp<Vector3>(target_->translate_, interTargetA, trackingDelay_) + (kOffset_ * rotateMat);
	}

	// オフセットの取得
	Vector3 offset = CalcOffset();
	// カメラの座標をオフセットを元に変更
	transform_.translate_ = interTarget_ + offset;

	// カメラのY座標が0以下の場合
	if (transform_.translate_.y < 0.05f) {
		// 0に補正する
		transform_.translate_.y = 0.05f;
	}

	// 基底クラスの更新を行う
	Camera::Update();
}

const Matrix4x4 FollowCamera::GetViewMatrixNoOffset()
{
	Matrix4x4 returnMatrix = noOffsetTransform_.GetMatWorld();
	returnMatrix = Matrix4x4::MakeInverse(returnMatrix);
	return returnMatrix;
}

void FollowCamera::OnCollision(Collider* collider)
{
	// 床と衝突していた場合
	if (collider->GetGameObject()->GetObjectTag() == TagFloor) {
		//targetAngleXOffset_ = KLib::Lerp<float>(0.1f, 0.0f, offset_.z, -40.0f);
	}
}

void FollowCamera::OnCollisionExit(Collider* collider)
{
	collider;
}

Vector3 FollowCamera::CalcOffset() const
{
	// 追従対象からカメラまでのオフセットを設定
	Vector3 offset = offset_;

	// カメラの角度から回転行列を生成
	Matrix4x4 rotateMat = Matrix4x4::MakeRotate(transform_.rotate_);

	// オフセットをカメラの回転に合わせて回転させる
	offset = offset * rotateMat;

	// 計算したオフセットを返す
	return offset;
}
