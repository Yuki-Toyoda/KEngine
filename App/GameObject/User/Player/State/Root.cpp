#include "Root.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void Root::Init()
{
	// ステート名の設定
	stateName_ = "Root";

	// 目標角度はプレイヤーの向き
	targetAngle_ = player_->transform_.rotate_.y;

	// 攻撃状態でない
	player_->SetIsAttacking(false);
	// コライダー無効
	player_->GetSwordLine()->isActive_ = false;
}

void Root::Update()
{

	// 移動中フラグをfalse
	isMoving_ = false;

	// スティックの方向を元に移動ベクトルを求める
	Vector3 move = input_->GetJoyStickInput(0);
	// 移動ベクトルが一定の長さになっていれば
	if (Vector3::Length(move) > deadZone_) {
		isMoving_ = true;
		
		// ロックオンが有効でないとき
		if (!player_->GetFollowCamera()->GetEnableZForcus()) {
			if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("01_Run")) {
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("01_Run", 0.25f, true);
			}
		}
		else { // 有効時
			// 横移動をしていない場合
			if (move.x < latralMovementThreshold_ && move.x > -latralMovementThreshold_) {
				// 移動ベクトルが前方方向だった場合
				if (move.z > 0.0f) {
					if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("01_Run")) {
						player_->skiningModels_["Player"]->animationManager_.PlayAnimation("01_Run", 0.1f, true);
					}
				}
				else { // 後方だった場合
					if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("03_LockOnMoveBack")) {
						player_->skiningModels_["Player"]->animationManager_.PlayAnimation("03_LockOnMoveBack", 0.15f, true);
					}
				}
			}
			else {
				// 右方向に移動している場合
				if (move.x > 0.0f) {
					if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("04_LockOnMoveRight")) {
						player_->skiningModels_["Player"]->animationManager_.PlayAnimation("04_LockOnMoveRight", 0.15f, true);
					}
				}
				else if(move.x < 0.0f){ // 左方向に移動している場合
					if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("05_LockOnMoveLeft")) {
						player_->skiningModels_["Player"]->animationManager_.PlayAnimation("05_LockOnMoveLeft", 0.15f, true);
					}
				}
			}
		}
	}

	// 移動中であれば
	if (isMoving_) {
		// プレイヤーに追従カメラがセットされている場合
		if (player_->GetFollowCamera() != nullptr) {
			
			if (move.z != -1.0f && !player_->GetFollowCamera()->GetEnableZForcus()) {
				player_->GetFollowCamera()->SetTargetAngle(player_->transform_.rotate_.y);
			}

			// カメラの角度から回転行列を生成
			Vector3 v = { 0.0f, 0.0f, 1.0f };
			Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->GetFollowCamera()->transform_.rotate_.y);
			// 移動ベクトルをカメラの角度に応じて回転させる
			move = (move * rotateMat);

			if (!player_->GetFollowCamera()->GetLockOn()->GetIsLockOn()) {
				// ターゲット座標
				Vector3 targetPos = player_->transform_.translate_ + (move * kMaxSpeed_);
				// 差分ベクトルを求める
				Vector3 sub = targetPos - player_->transform_.translate_;

				// 目標角度の取得
				targetAngle_ = std::atan2(sub.x, sub.z);
			}

		}

		// 移動ベクトルに最大速度を掛ける
		if (player_->GetFollowCamera()->GetEnableZForcus()) {
			move = move * kMaxZforcusSpeed_;
		}
		else {
			move = move * kMaxSpeed_;
		}
	}
	else {
		// ロックオンが有効でないとき
		if (!player_->GetFollowCamera()->GetEnableZForcus()) {
			// 
			if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("00_Idle")) {
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("00_Idle", 0.25f, true);
			}
		}
		else { // 有効時
			// 
			if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("02_LockOnIdle")) {
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("02_LockOnIdle", 0.15f, true);
			}
		}
	}

	// プレイヤーを移動方向に回転させる
	if (player_->transform_.rotate_.y != targetAngle_ && !player_->GetFollowCamera()->GetEnableZForcus()) {
		player_->transform_.rotate_.y = KLib::LerpShortAngle(player_->transform_.rotate_.y, targetAngle_, 0.1f);
	}

	// 移動ベクトルに基づいてプレイヤーを動かす
	player_->transform_.translate_ += move;
}

void Root::DisplayImGui()
{
	ImGui::DragFloat("TargetAngle", &targetAngle_, 0.0f);
}
