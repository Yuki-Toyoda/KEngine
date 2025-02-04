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
	player_->GetAttackManager()->SetIsAttacking(false);
	// コライダー無効
	player_->GetSwordLine()->GetCollider()->SetIsActive(false);
}

void Root::Update()
{
	// 移動中フラグをfalse
	isMoving_ = false;

	// 左スティックの方向を元に移動ベクトルを求める
	Vector3 move = input_->GetJoyStickInput(0);

	// 移動ベクトルが一定の長さになっていれば
	if (Vector3::Length(move) > deadZone_) {
		// 移動中状態へ
		isMoving_ = true;
	}
	else {
		// ロックオンが有効でないとき
		if (!player_->GetFollowCamera()->GetEnableZForcus()) {
			player_->skiningModels_["Player"]->animationManager_.PlayAnimation("Idle", idleAnimTransitionTime_, true, true);
		}
		else { // 有効時
			player_->skiningModels_["Player"]->animationManager_.PlayAnimation("LockOnIdle", zForcusIdleAnimTrainsitionTime_, true, true);
		}
	}

	// 移動中でなければ早期リターン
	if (!isMoving_) { return; }

	// プレイヤーに追従カメラがセットされている場合
	if (player_->GetFollowCamera() != nullptr) {

		// プレイヤーの真後ろに追従するように目標角度を選定
		if (move.z != -1.0f && !player_->GetFollowCamera()->GetEnableZForcus()) {
			player_->GetFollowCamera()->SetTargetAngle(player_->transform_.rotate_.y);
		}

		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Matrix4x4::MakeRotateY(player_->GetFollowCamera()->transform_.rotate_.y);
		// 移動ベクトルをカメラの角度に応じて回転させる
		move = Vector3::Normalize((move * rotateMat));

		if (!player_->GetFollowCamera()->GetLockOn()->GetIsLockOn()) {
			// ターゲット座標
			Vector3 targetPos = player_->transform_.translate_ + (move * kMaxSpeed_);
			// 差分ベクトルを求める
			Vector3 sub = targetPos - player_->transform_.translate_;

			// 目標角度の取得
			targetAngle_ = std::atan2(sub.x, sub.z);
		}

	}

	// ロックオンが有効でないとき
	if (!player_->GetFollowCamera()->GetEnableZForcus()) {
		player_->skiningModels_["Player"]->animationManager_.PlayAnimation("Run", runAnimTransitionTime_, true, true);
	}
	else { // 有効時
		// 横移動をしていない場合
		if (move.x < latralMovementThreshold_ && move.x > -latralMovementThreshold_) {
			// 移動ベクトルが前方方向だった場合
			if (move.z > 0.0f) {
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("Run", runAnimTransitionTime_, true, true);
			}
			else { // 後方だった場合
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("LockOnMoveBack", zForcusRunAnimTrainsitionTime_, true, true);
			}
		}
		else {
			// 右方向に移動している場合
			if (move.x > 0.0f) {
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("LockOnMoveRight", zForcusRunAnimTrainsitionTime_, true, true);
			}
			else if (move.x < 0.0f) { // 左方向に移動している場合
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("LockOnMoveLeft", zForcusRunAnimTrainsitionTime_, true, true);
			}
		}
	}

	// 現在の移動ベクトルを保持
	speed_ = move;

	// 移動ベクトルに最大速度を掛ける
	if (player_->GetFollowCamera()->GetEnableZForcus()) {
		move = move * kMaxZforcusSpeed_;
	}
	else {
		move = move * kMaxSpeed_;
	}

	// プレイヤーを移動方向に回転させる
	if (player_->transform_.rotate_.y != targetAngle_ && !player_->GetFollowCamera()->GetEnableZForcus()) {
		player_->transform_.rotate_.y = KLib::LerpShortAngle(player_->transform_.rotate_.y, targetAngle_, targetAngleConrrectSpeed_);
	}

	// 移動ベクトルに基づいてプレイヤーを動かす
	player_->transform_.translate_ += move;
}

void Root::DisplayImGui()
{
	
}
