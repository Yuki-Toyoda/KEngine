#include "DashState.h"
#include "../Player.h"

void DashState::Init()
{
	name_ = "Dash";

	velocity_ = Math::Normalize(player_->GetMoveDirect());
	moveDirect_ = Math::Normalize(player_->GetMoveDirect());
	float endFrame = 10.0f;
	dashTimer_ = KLib::DeltaTimer(1.0f/ endFrame);
	dashTimer_.Start();
}

void DashState::Update()
{
	dashTimer_.Update();

	if (dashTimer_.GetIsFinish()) {
		player_->ChangeState(std::make_unique<RootState>());
		return;
	}

	float dashPower = 3.0f;
	velocity_ += (moveDirect_ * player_->GetMoveAcceleration()) * dashPower;

	player_->SetVelocity(velocity_);

}

void DashState::DisplayImGui()
{
}
