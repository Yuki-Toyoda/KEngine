#include "RootState.h"
#include "../Player.h"
void RootState::Init()
{
	// 名称設定
	name_ = "Root";
}

void RootState::Update()
{
	//// 移動ベクトル初期化
	//velocity_ = { 0.0f,0.0f,0.0f };

	// 入力マネージャから移動ベクトルを取得
	velocity_ =InputManager::Move(velocity_);
	//移動ベクトルを減算して加減速を付ける
	velocity_ = velocity_ *player_-> decelerationRate;
	if (std::abs(velocity_.x) <= 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::abs(velocity_.z) <= 0.01f) {
		velocity_.z = 0.0f;
	}
	//移動ベクトルをプレイヤーに渡す
	player_->SetVelocity(Math::Normalize( velocity_));
	//攻撃ボタンを押して且つ攻撃できるときにStateをAtackに
	if (InputManager::Atack()&&player_->GetAbsorptionCount()>=kMinCount) {
		player_->ChangeState(std::make_unique<AtackState>());
		return;
	}
}

void RootState::DisplayImGui()
{
}
