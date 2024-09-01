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
		
		// ロックオンが有効でないとき
		if (!player_->followCamera_->GetEnableZForcus()) {
			if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("01_Run")) {
				player_->skiningModels_[0]->animationManager_.PlayAnimation("01_Run");
			}
		}
		else { // 有効時
			// 横移動をしていない場合
			if (move.x < 0.75f && move.x > -0.75f) {
				// 移動ベクトルが前方方向だった場合
				if (move.z > 0.0f) {
					if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("01_Run")) {
						player_->skiningModels_[0]->animationManager_.PlayAnimation("01_Run");
					}
				}
				else { // 後方だった場合
					if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("09_LockOnMoveBack")) {
						player_->skiningModels_[0]->animationManager_.PlayAnimation("09_LockOnMoveBack");
					}
				}
			}
			else {
				// 右方向に移動している場合
				if (move.x > 0.0f) {
					if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("10_LockOnMoveRight")) {
						player_->skiningModels_[0]->animationManager_.PlayAnimation("10_LockOnMoveRight");
					}
				}
				else if(move.x < 0.0f){ // 左方向に移動している場合
					if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("11_LockOnMoveLeft")) {
						player_->skiningModels_[0]->animationManager_.PlayAnimation("11_LockOnMoveLeft");
					}
				}
			}
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
		// ロックオンが有効でないとき
		if (!player_->followCamera_->GetEnableZForcus()) {
			// 
			if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("00_Idle")) {
				player_->skiningModels_[0]->animationManager_.PlayAnimation("00_Idle");
			}
		}
		else { // 有効時
			// 
			if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("08_LockOnIdle")) {
				player_->skiningModels_[0]->animationManager_.PlayAnimation("08_LockOnIdle");
			}
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
