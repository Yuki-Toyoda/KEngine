#include "AtackState.h"
#include "../Player.h"
void AtackState::Init()
{
	// ステート名の初期化
	name_ = "Atack";
}

void AtackState::Update()
{
	// プレイヤーの攻撃処理を行う
	player_->Atack();
	//攻撃が終わったら吹き飛ばされる
	player_->ChangeState(std::make_unique<BlowAwayState>());
	return;
}

void AtackState::DisplayImGui()
{
}
