#include "AtackState.h"
#include "../Player.h"
void AtackState::Init()
{
	name_ = "Atack";
}

void AtackState::Update()
{
	player_->Atack();
	//攻撃が終わったら吹き飛ばされる
	player_->ChangeState(std::make_unique<BlowAwayState>());
	return;
}

void AtackState::DisplayImGui()
{
}
