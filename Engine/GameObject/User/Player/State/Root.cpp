#include "Root.h"
#include "../Player.h"
#include "../../FollowCamera/FollowCamera.h"

void Root::Init()
{
	
}

void Root::Update()
{
	// 移動中フラグをfalse
	isMoving_ = false;

	// スティックの方向を元に移動ベクトルを求める
	Vector3 move = {
		(float)player_->joyState_.Gamepad.sThumbLX, 0.0f,
		(float)player_->joyState_.Gamepad.sThumbLY };
	// 移動ベクトルが一定の長さになっていれば
	if (Math::Length(move) > deadZone_) {
		isMoving_ = true;
	}

	if (!player_->followCamera_->GetEnableZForcus()) {
		player_->followCamera_->SetTargetAngle(player_->followCamera_->transform_.rotate_.y);
	}

	// 移動中であれば
	if (isMoving_) {
		// 移動ベクトルの正規化を行う
		move = Math::Normalize(move);

		// プレイヤーに追従カメラがセットされている場合
		if (player_->followCamera_ != nullptr) {
			
			if (move.z != -1.0f && !player_->followCamera_->GetEnableZForcus()) {
				player_->followCamera_->SetTargetAngle(player_->transform_.rotate_.y);
			}

			// カメラの角度から回転行列を生成
			Vector3 v = { 0.0f, 0.0f, 1.0f };
			Matrix4x4 rotateMat = Math::MakeRotateYMatrix(player_->followCamera_->transform_.rotate_.y);
			// 移動ベクトルをカメラの角度に応じて回転させる
			move = Math::Transform(move, rotateMat);
			// ターゲット座標
			Vector3 targetPos = player_->transform_.translate_ + (move * kMaxSpeed_);
			// 差分ベクトルを求める
			Vector3 sub = targetPos - player_->transform_.translate_;
			sub = Math::Normalize(sub);
			// 目標角度の取得
			targetAngle_ = Math::Angle({ 0.0f, 0.0f, 1.0f }, sub);
		}

		if (player_->transform_.rotate_.y != targetAngle_ && !player_->followCamera_->GetEnableZForcus()) {
			player_->transform_.rotate_.y = Math::LerpShortAngle(player_->transform_.rotate_.y, targetAngle_, 0.1f);
		}

		// 移動ベクトルに最大速度を掛ける
		move = move * kMaxSpeed_;
	}

	// 目標角度と現在角度が一致していたら移動させる
	/*float subDirection = player_->transform_.rotate_.y - targetAngle_;
	if (subDirection < 0.0f) {
		subDirection *= -1.0f;
	}*/

	player_->transform_.translate_ += move;
}

void Root::DisplayImGui()
{
	ImGui::DragFloat("TargetAngle", &targetAngle_, 0.0f);
}
