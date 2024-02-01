#include "AtackState.h"
#include "../Player.h"
void AtackState::Init()
{
	// ステート名の初期化
	name_ = "Atack";
	// プレイヤーの攻撃処理を行う
	player_->Atacking();
}

void AtackState::Update()
{
	
	
}

void AtackState::DisplayImGui()
{
}
