#include "PushUpHitState.h"
#include "../Player.h"
void PushUpHitState::Init()
{
	name_ = "hitPushUp";
	velocity_ =Math::Normalize(  player_->GetPushUpPos()-player_->transform_.translate_)*2.0f;
}

void PushUpHitState::Update()
{
	velocity_.y = 0.0f;
	velocity_ = velocity_ * player_->decelerationRate;
	if (std::abs(velocity_.x) <= 0.01f && std::abs(velocity_.z) <= 0.01f) {
		player_->ChangeState(std::make_unique<RootState>());
	}
}

void PushUpHitState::DisplayImGui()
{
}
