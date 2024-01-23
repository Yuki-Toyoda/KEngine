#include "RootState.h"
#include "../Player.h"

void RootState::Init()
{
	// 名称設定
	name_ = "Root";
}

void RootState::Update()
{
	// 移動処理
	Move();

	//攻撃ボタンを押して且つ攻撃できるときにStateをAtackに
	if (InputManager::Atack()&&player_->GetAbsorptionCount()>=kMinCount) {
		Attack();
	}
}

void RootState::DisplayImGui()
{
}

void RootState::Move()
{
	//// 移動ベクトル初期化
	//velocity_ = { 0.0f,0.0f,0.0f };

	// 入力マネージャから移動方向ベクトルを取得
	Vector3 move = Vector3(0.0f, 0.0f, 0.0f);
	move = InputManager::Move(move);

	if (move.x != 0.0f || move.y != 0.0f || move.z != 0.0f) {
		// プレイヤーの移動ベクトルと加速度を乗算
		velocity_ += move * player_->GetMoveAcceleration();
	}
	else {
		// 無操作状態の場合は現在速度を0に近づけていく
		velocity_ = KLib::Lerp<Vector3>(velocity_, Vector3(0.0f, 0.0f, 0.0f), player_->GetDecayAcceleration());
	}

	// 各軸の速度ベクトルが最大加速度を超過していた場合
	if (velocity_.x > player_->GetMaxMoveAcceleration()) {
		velocity_.x = player_->GetMaxMoveAcceleration();
	}else if (velocity_.x < -player_->GetMaxMoveAcceleration()) {
		velocity_.x = -player_->GetMaxMoveAcceleration();
	}// x軸
	if (velocity_.z > player_->GetMaxMoveAcceleration()) {
		velocity_.z = player_->GetMaxMoveAcceleration();
	}
	else if (velocity_.z < -player_->GetMaxMoveAcceleration()) {
		velocity_.z = -player_->GetMaxMoveAcceleration();
	}// z軸
	
	//移動ベクトルをプレイヤーに渡す
	player_->SetVelocity(velocity_);
}

void RootState::Attack()
{
	player_->SetIsAtack(true);
	player_->ChangeState(std::make_unique<AtackState>());
	return;
}
