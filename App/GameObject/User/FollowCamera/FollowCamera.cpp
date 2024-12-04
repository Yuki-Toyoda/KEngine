#include "FollowCamera.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void FollowCamera::Init()
{
	// 基底クラスの初期化を行う
	Camera::Init();

	// ブルームの閾値を変更しておく
	ppProcessor_.bloom_.threshold_ = 0.25f;

	// オフセットなしのトランスフォームの初期化
	noOffsetTransform_.Init();
}

void FollowCamera::Update()
{
	// このカメラを使用中でない場合早期リターン
	if (!isUseThisCamera_) { return; }

	// 左トリガーがトリガーされた、かつZ注目が有効になっていない場合
	if (input_->InspectTrigger(TRIGGER, 0) && !enableZForcus_) {
		// X軸の目標角度は0に
		targetAngleX_ = 0.0f;
		// 目標角度をプレイヤーの真後ろに
		targetAngleY_ = target_->rotate_.y;
		// カメラを一時的に操作不能に
		isCanControll_ = false;

		// Z注目有効
		enableZForcus_ = true;
	}

	// Z注目有効時
	if (enableZForcus_) {
		// 注目時の更新
		ZForcusUpdate();
		// 注目時のビネット更新
		ForcusVignetteUpdate();
	}
	else { // 有効になっていない場合
		// 角度補正速度を設定
		correctionSpeed_ = normalCorrectionSpeed_;

		// X軸の目標角度は0に
		targetAngleX_ = 0.0f;
		// 目標角度を現在の角度に
		targetAngleY_ = transform_.rotate_.y;

		// カメラ操作を行う
		ControllUpdate();
	}

	// オフセットを線形補間で動かす
	offset_ = KLib::Lerp<Vector3>(offset_, targetOffset_, 0.1f);

	// 追従対象の更新
	UpdateTarget();

	// パリィ演出を行う場合
	if (isParryStaging_) {
		// パリィ演出の更新
		ParryBlurUpdate();
	}
}

void FollowCamera::DisplayImGui()
{
	// 基底クラスのImGuiを表示
	Camera::DisplayImGui();

	// 感度調整
	ImGui::DragFloat2("Seisitivity", &sensitivity_.x, 0.001f, 0.001f);

	// 目標角度表示
	ImGui::DragFloat("TargetAngleX", &targetAngleX_);
	ImGui::DragFloat("TargetAngleY", &targetAngleY_);
	ImGui::DragFloat("DirectionOffset", &directionOffset_);
	// 目標角度表示
	ImGui::DragFloat3("offset", &offset_.x, 0.05f);

	// 補間系パラメータの調整
	ImGui::DragFloat("maxLockOnControllAngleX", &maxLockOnControllAngleX_, 0.001f);
	ImGui::DragFloat("minLockOnControllAngleX", &minLockOnControllAngleX_, 0.001f);
	ImGui::DragFloat("maxAddLockOnXAngleZOffset", &maxAddLockOnXAngleZOffset_, 0.001f);
	ImGui::DragFloat("minAddLockOnXAngleZOffset", &minAddLockOnXAngleZOffset_, 0.001f);
	ImGui::DragFloat("maxAddLockOnYAngleZOffset", &maxAddLockOnYAngleZOffset_, 0.001f);
	ImGui::DragFloat("minAddLockOnYAngleZOffset", &minAddLockOnYAngleZOffset_, 0.001f);
	ImGui::DragFloat("maxAddLockOnDistanceZOffset", &maxAddLockOnDistanceZOffset_, 0.001f);
	ImGui::DragFloat("minAddLockOnDistanceZOffset", &minAddLockOnDistanceZOffset_, 0.001f);

	ImGui::DragFloat("DirectionOffset", &directionOffset_);
	ImGui::DragFloat("RotateOffset", &kOffsetRotate_);
}

void FollowCamera::StartParryBlur(const float stagingTime, const float endStagingTime, const float blurStrength)
{
	// ブラー演出中状態に
	isParryStaging_ = true;
	// ブラー演出用タイマーを開始する
	parryBlurTimer_.Start(stagingTime);
	// 終了時間の取得
	parryStagingEndTime_ = endStagingTime;
	// ブラー強さの設定
	parryBlurStrength_ = blurStrength;
	// 一応終了演出フラグをfalseにしておく
	isParryEndStaging_ = false;
}

void FollowCamera::UpdateTarget()
{
	// 追従対象が存在するときのみ
	if (target_) {
		if (lockOn_->GetIsLockOn()) {
			// 遷移後の座標を求める
			interTarget_ = KLib::Lerp<Vector3>(interTarget_, lockOnTranslate_, zForcusTrackingDelay_);
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

	// カメラのY座標が指定の座標以下の場合
	if (transform_.translate_.y < minPositionY_) {
		// 最小位置に補正する
		transform_.translate_.y = minPositionY_;
		// 床に触れている状態に
		isTouchFloor_ = true;
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

void FollowCamera::ZForcusUpdate()
{
	// ロックオンが有効になっている場合
	if (lockOn_->EnableLockOn()) {
		// 目標角度になるまでは補正を行う
		if (std::abs(transform_.rotate_.y - targetAngleY_) > 0.1f && !isCanControll_) {
			// ロックオン対象の座標
			Vector3 targetPos = lockOn_->target_->transform_.translate_;
			// 追従対象からロックオン対象への差分ベクトル
			Vector3 sub = targetPos - target_->translate_;

			// ロックオン時の座標を計算
			lockOnTranslate_ = targetPos - (sub / 2.0f);

			// 回転方向を指定
			targetAngleY_ = transform_.rotate_.y;

			// オフセットのY軸を0に
			targetOffset_.y = 0.0f;
			targetOffset_.z = -Vector3::Length(sub / zOffsetRatio_) + kOffset_.z;

			// 角度補正速度を設定
			correctionSpeed_ = zStartForcusCorrectionSpeed_;

			// 回転させる
			transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngleY_, correctionSpeed_);

			float addZOffset = 0.0f;

			// X軸方向の回転でオフセットを変更する
			if (transform_.rotate_.x > 0.0f) {
				targetOffset_.y = KLib::Lerp<float>(minLockOnHeight_, maxLockOnHeight_, transform_.rotate_.x, maxLockOnControllAngleX_);
				addZOffset += KLib::Lerp<float>(minAddLockOnXAngleZOffset_, maxAddLockOnXAngleZOffset_, transform_.rotate_.x, maxLockOnControllAngleX_);
			}
			// Y軸方向の回転でオフセットを変更する
			directionOffset_ = std::abs(transform_.rotate_.y - targetAngleY_);
			addZOffset += KLib::Lerp<float>(minAddLockOnYAngleZOffset_, maxAddLockOnYAngleZOffset_, directionOffset_, 2.75f);

			// 敵との距離によってオフセットを変更する
			addZOffset += KLib::Lerp<float>(minAddLockOnDistanceZOffset_, maxAddLockOnDistanceZOffset_, std::abs(Vector3::Length(sub)), 40.0f);

			targetOffset_.z += addZOffset;
		}
		else {
			// 角度補正速度を設定
			correctionSpeed_ = zEnemyForcusCorrectionSpeed_;

			// 操作可能状態に
			isCanControll_ = true;
		}

		ForcusControllUpdate();
	}
	else { // 有効になっていない場合
		// 角度補正速度を設定
		correctionSpeed_ = zForcusCorrectionSpeed_;

		// 目標角度になるまでは補正を行う
		if (std::abs(transform_.rotate_.y - targetAngleY_) > 0.1f && !isCanControll_) {
			// 回転させる
			transform_.rotate_.x = KLib::LerpShortAngle(transform_.rotate_.x, targetAngleX_, correctionSpeed_);
			transform_.rotate_.y = KLib::LerpShortAngle(transform_.rotate_.y, targetAngleY_, correctionSpeed_);

			// 位置を元に戻す
			targetOffset_.y = KLib::Lerp<float>(offset_.y, kOffset_.y, correctionSpeed_);
			targetOffset_.z = KLib::Lerp<float>(offset_.z, kOffset_.z, correctionSpeed_);
		}
		else {
			// 操作可能状態に
			isCanControll_ = true;
		}

		// カメラ操作を行う
		ControllUpdate();
	}

	// 左トリガーが離されている場合
	if (!input_->InspectTrigger(PRESS, 0)) {
		// Z注目をしていないに
		enableZForcus_ = false;
		// ロックオン方向の選定
		lockOnDirectionSetUp_ = false;
		// カメラ操作を可能な状態に
		isCanControll_ = true;

		// ロックオンを無効
		lockOn_->DisableLockOn();
	}
}

void FollowCamera::ForcusVignetteUpdate()
{
	// Z注目有効時にビネットエフェクトを徐々にかける
	if (enableZForcus_) {
		ppProcessor_.vignette_.intensity_ = KLib::Lerp<float>(ppProcessor_.vignette_.intensity_, vignetteStrength_, vignetteT_);
	}
	else { // 無効時は徐々に下げる
		ppProcessor_.vignette_.intensity_ = KLib::Lerp<float>(ppProcessor_.vignette_.intensity_, 0.0f, vignetteT_);
	}
}

void FollowCamera::ControllUpdate()
{
	// 操作可能状態でない場合早期リターン
	if (!isCanControll_) { return; }

	// 右スティックの入力を取得
	Vector3 rStickInput = input_->GetJoyStickInput(1);
	// カメラを回転させる
	transform_.rotate_.y += rStickInput.x * sensitivity_.x;
	transform_.rotate_.x -= rStickInput.z * sensitivity_.y;
	
	// x軸の回転角度が一定値を上回った場合
	if (transform_.rotate_.x > maxControllAngleX_) {
		transform_.rotate_.x = maxControllAngleX_;
	}
	if (transform_.rotate_.x < minControllAngleX_) {
		transform_.rotate_.x = minControllAngleX_;
	}

	if (transform_.rotate_.x < 0.0f) {
		targetOffset_.z = KLib::Lerp<float>(kOffset_.z, minlerpOffsetZ_, transform_.rotate_.x, -0.2f);
		targetOffset_.y = KLib::Lerp<float>(kOffset_.y, minlerpOffsetY_, transform_.rotate_.x, -0.2f);
	}
	else {
		targetOffset_.z = KLib::Lerp<float>(kOffset_.z, maxlerpOffsetZ_, transform_.rotate_.x, maxControllAngleX_);
		targetOffset_.y = kOffset_.y;
	}
}

void FollowCamera::ForcusControllUpdate()
{
	// 操作可能状態でない場合早期リターン
	if (!isCanControll_) { return; }

	// ロックオン対象の座標
	Vector3 targetPos = lockOn_->target_->transform_.translate_;
	// 追従対象からロックオン対象への差分ベクトル
	Vector3 sub = targetPos - target_->translate_;

	// ロックオン時の座標を計算
	lockOnTranslate_ = targetPos - (sub / 2.0f);

	// 方向ベクトルを元にプレイヤーがいる角度を求める
	targetAngleY_ = std::atan2(sub.x, sub.z);

	// オフセットのY軸を0に
	targetOffset_.y = 0.0f;
	targetOffset_.z = -Vector3::Length(sub / zOffsetRatio_) + kOffset_.z;

	// 右スティックの入力を取得
	Vector3 rStickInput = input_->GetJoyStickInput(1);
	// カメラを回転させる
	transform_.rotate_.y += rStickInput.x * sensitivity_.x;
	transform_.rotate_.x -= rStickInput.z * sensitivity_.y;

	// x軸の回転角度が一定値を上回った場合
	if (transform_.rotate_.x > maxLockOnControllAngleX_) {
		transform_.rotate_.x = maxLockOnControllAngleX_;
	}
	if (transform_.rotate_.x < minLockOnControllAngleX_) {
		transform_.rotate_.x = minLockOnControllAngleX_;
	}

	float addZOffset = 0.0f;

	// X軸方向の回転でオフセットを変更する
	if (transform_.rotate_.x > 0.0f) {
		targetOffset_.y = KLib::Lerp<float>(minLockOnHeight_, maxLockOnHeight_, transform_.rotate_.x, maxLockOnControllAngleX_);
		addZOffset += KLib::Lerp<float>(minAddLockOnXAngleZOffset_, maxAddLockOnXAngleZOffset_, transform_.rotate_.x, maxLockOnControllAngleX_);
	}
	// Y軸方向の回転でオフセットを変更する
	float rotateY = transform_.rotate_.y;
	float targetY = targetAngleY_;

	// 回転角の超過判定
	if (rotateY < targetY) {
		if (targetY + std::abs(rotateY) > static_cast<float>(std::numbers::pi)) {
			rotateY = std::abs(rotateY);
			if (rotateY > targetY) {
				directionOffset_ = rotateY - targetY;
			}
			else {
				directionOffset_ = targetY - rotateY;
			}
		}
		else {
			directionOffset_ = targetY - rotateY;
		}
		
	}
	else {
		if (rotateY + std::abs(targetY) > static_cast<float>(std::numbers::pi)) {
			targetY = std::abs(targetY);
			if (rotateY > targetY) {
				directionOffset_ = rotateY - targetY;
			}
			else {
				directionOffset_ = targetY - rotateY;
			}
		}
		else {
			directionOffset_ = rotateY - targetY;
		}
	}

	addZOffset += KLib::Lerp<float>(minAddLockOnYAngleZOffset_, maxAddLockOnYAngleZOffset_, directionOffset_, static_cast<float>(std::numbers::pi));

	// 敵との距離によってオフセットを変更する
	addZOffset += KLib::Lerp<float>(minAddLockOnDistanceZOffset_, maxAddLockOnDistanceZOffset_, std::abs(Vector3::Length(sub)), 40.0f);

	targetOffset_.z += addZOffset;
}

void FollowCamera::ParryBlurUpdate()
{
	// パリィ演出タイマーが終了していない場合
	if (!parryBlurTimer_.GetIsFinish() && !isParryEndStaging_) {
		// ブラーを徐々に線形補間で強くしていく
		ppProcessor_.radialBlur_.data_.blurStrength = 
			KLib::Lerp<float>(ppProcessor_.radialBlur_.data_.blurStrength, parryBlurStrength_, parryBlurTimer_.GetProgress());
	}
	else if(!isParryEndStaging_){
		// ブラーの強度を固定
		ppProcessor_.radialBlur_.data_.blurStrength = parryBlurStrength_;

		// 終了演出開始
		isParryEndStaging_ = true;
		// パリィ演出タイマーを再度開始する
		parryBlurTimer_.Start(parryStagingEndTime_);
	}
	else if (!parryBlurTimer_.GetIsFinish() && isParryEndStaging_) {
		// ブラーを徐々に線形補間で弱くしていく
		ppProcessor_.radialBlur_.data_.blurStrength =
			KLib::Lerp<float>(ppProcessor_.radialBlur_.data_.blurStrength, 0.0f, parryBlurTimer_.GetProgress());
	}
	else {
		// パリィ演出終了
		isParryStaging_ = false;
		// 終了演出フラグfalse
		isParryEndStaging_ = false;
	}

	// タイマーを更新する
	parryBlurTimer_.Update();
}


