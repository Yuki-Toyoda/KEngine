#include "PushUpAtackState.h"
#include "../IEnemy.h"
void PushUpAtackState::Init()
{
	name_ = "PushUp";
	enemy_->StateNumber_++;
}

void PushUpAtackState::Update()
{
	for ( PushUp* pushUp : enemy_->pushUp_) {
		pushUp->SetActive();
	}
	enemy_->SetWaitTime(enemy_->GetWaitPushUp());
	enemy_->ChangeState(std::make_unique<WaitTimeState>());
	return;
}

void PushUpAtackState::DisplayImGui()
{
}
