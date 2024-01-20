#include "BlowAwayState.h"
#include "../Player.h"
void BlowAwayState::Init()
{
	name_ = "BlowAway";
	//攻撃状態を解除
	
	velocity_ = player_->transform_.translate_ - Vector3(0.0f, 0.0f, 0.0f);
}

void BlowAwayState::Update()
{
	player_->SetIsAtack(false);
	velocity_.y = 0.0f;
	//回転しながら壁に向かって吹き飛ぶ
	player_->SetVelocity(Math::Normalize(velocity_));
	
	player_->transform_.rotate_.y += 0.1f;
}

void BlowAwayState::DisplayImGui()
{
}
