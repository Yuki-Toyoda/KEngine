#include "Root.h"
#include "../Player.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../LockOn/LockOn.h"
#include "../../Enemy/Enemy.h"

void Root::Init()
{
	stateName_ = "Root";

	// 目標角度はプレイヤーの向き
	targetAngle_ = player_->transform_.rotate_.y;

	// 線の座標を戻す
	player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };

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
	if (Vector3::Length(move) > deadZone_) {
		isMoving_ = true;
		
		if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("01_Run")) {
			player_->skiningModels_[0]->animationManager_.PlayAnimation("01_Run");
		}
	}

	// 移動中であれば
	if (isMoving_) {
		// 移動ベクトルの正規化を行う
		move = Vector3::Normalize(move);

		// プレイヤーに追従カメラがセットされている場合
		if (player_->followCamera_ != nullptr) {
			
			if (move.z != -1.0f && !player_->followCamera_->GetEnableZForcus()) {
				player_->followCamera_->SetTargetAngle(player_->transform_.rotate_.y);
			}

			// カメラの角度から回転行列を生成
			Vector3 v = { 0.0f, 0.0f, 1.0f };
			Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->followCamera_->transform_.rotate_.y);
			// 移動ベクトルをカメラの角度に応じて回転させる
			move = (move * rotateMat);

			if (!player_->followCamera_->GetLockOn()->GetIsLockOn()) {
				// ターゲット座標
				Vector3 targetPos = player_->transform_.translate_ + (move * kMaxSpeed_);
				// 差分ベクトルを求める
				Vector3 sub = targetPos - player_->transform_.translate_;

				// 目標角度の取得
				targetAngle_ = std::atan2(sub.x, sub.z);
			}

		}

		// 移動ベクトルに最大速度を掛ける
		if (player_->followCamera_->GetEnableZForcus()) {
			move = move * kMaxZforcusSpeed_;
		}
		else {
			move = move * kMaxSpeed_;
		}
	}
	else {
		if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("00_Idle")) {
			player_->skiningModels_[0]->animationManager_.PlayAnimation("00_Idle");
		}
	}

	// プレイヤーを移動方向に回転させる
	if (player_->transform_.rotate_.y != targetAngle_ && !player_->followCamera_->GetEnableZForcus()) {
		player_->transform_.rotate_.y = KLib::LerpShortAngle(player_->transform_.rotate_.y, targetAngle_, 0.1f);
	}

	player_->transform_.translate_ += move;
}

void Root::DisplayImGui()
{
	ImGui::DragFloat("TargetAngle", &targetAngle_, 0.0f);
}
