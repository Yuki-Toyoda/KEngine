#include "WaitTimeState.h"
#include "../IEnemy.h"

void WaitTimeState::Init()
{
	name_ = "Wait";
	waitTimer_.Start(enemy_->GetWaitTime());
}

void WaitTimeState::Update()
{
	waitTimer_.Update();
	if (waitTimer_.GetIsFinish() && enemy_->StateNumber_ < enemy_->stateList_.at(enemy_->patternNumber_).size() - 1) {
		enemy_->StateNumber_++;

		enemy_->ChangeState(enemy_->stateList_.at(enemy_->patternNumber_).at(enemy_->StateNumber_)->name_);
		return;
	}
	if (enemy_->StateNumber_ == enemy_->stateList_.at(enemy_->patternNumber_).size() - 1) {
		enemy_->StateNumber_ = 0;
		enemy_->ChangeState(enemy_->stateList_.at(enemy_->patternNumber_).at(enemy_->StateNumber_)->name_);
		return;
	}
}

void WaitTimeState::DisplayImGui()
{
}
