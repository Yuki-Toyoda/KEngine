#include "AtackState.h"
#include "../Player.h"
void AtackState::Init()
{
	name_ = "Atack";
}

void AtackState::Update()
{
	player_->Atack();
	
}

void AtackState::DisplayImGui()
{
}
