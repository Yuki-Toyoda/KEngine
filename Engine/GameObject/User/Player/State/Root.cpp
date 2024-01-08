#include "Root.h"
#include "../Player.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../LockOn/LockOn.h"
#include "../../Enemy/Enemy.h"

void Root::Init()
{
	// アニメーションをループする状態に
	player_->playerAnim_->isLoop_ = true;

	// 目標角度はプレイヤーの向き
	targetAngle_ = player_->transform_.rotate_.y;

	// 線の座標を戻す
	player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };

	// プレイヤーアニメーションを変更
	if (player_->playerAnim_->GetReadingParameterName() != "Player_Idle") {
		player_->playerAnim_->ChangeParameter("Player_Idle", true);
	}
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

	// 移動中であれば
	if (isMoving_) {
		// 移動ベクトルの正規化を行う
		move = Math::Normalize(move);

		// プレイヤーアニメーションを変更
		if (player_->playerAnim_->GetReadingParameterName() != "Player_Run") {
			player_->playerAnim_->ChangeParameter("Player_Run", true);
		}

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
		// プレイヤーアニメーションを変更
		if (player_->playerAnim_->GetReadingParameterName() != "Player_Idle") {
			player_->playerAnim_->ChangeParameter("Player_Idle", true);
		}
	}

	//if (enemy_->followCamera_->GetLockOn()->GetIsLockOn()) {
	//	// ロックオン対象の座標
	//	Vector3 targetPos = enemy_->followCamera_->GetLockOn()->target_->transform_.translate_;
	//	// 追従対象からロックオン対象への差分ベクトル
	//	Vector3 sub = targetPos - enemy_->transform_.translate_;
	//	// 方向ベクトルを元にプレイヤーがいる角度を求める
	//	targetAngleY_ = std::atan2(sub.x, sub.z);
	//}

	// プレイヤーを移動方向に回転させる
	if (player_->transform_.rotate_.y != targetAngle_ && !player_->followCamera_->GetEnableZForcus()) {
		player_->transform_.rotate_.y = Math::LerpShortAngle(player_->transform_.rotate_.y, targetAngle_, 0.1f);
	}

	// 目標角度と現在角度が一致していたら移動させる
	/*float subDirection = enemy_->transform_.rotate_.y - targetAngleY_;
	if (subDirection < 0.0f) {
		subDirection *= -1.0f;
	}*/

	player_->transform_.translate_ += move;
}

void Root::DisplayImGui()
{
	ImGui::DragFloat("TargetAngle", &targetAngle_, 0.0f);
}
