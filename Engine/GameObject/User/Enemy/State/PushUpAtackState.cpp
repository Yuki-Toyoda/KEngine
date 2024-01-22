#include "PushUpAtackState.h"
#include "../IEnemy.h"
void PushUpAtackState::Init()
{
	name_ = "PushUp";
}

void PushUpAtackState::Update()
{
	for ( PushUp* pushUp : enemy_->pushUp_) {
		pushUp->SetActive();
	}
	enemy_->ChangeState(std::make_unique<EnemySampleState>());
	return;
}

void PushUpAtackState::DisplayImGui()
{
}
