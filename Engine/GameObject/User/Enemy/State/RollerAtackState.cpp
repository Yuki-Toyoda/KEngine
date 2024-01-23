#include "RollerAtackState.h"
#include "../IEnemy.h"
void RollerAtackState::Init()
{
	name_ = "Roller";
	//隕石を生成してstateを変更
	Roller* roller = gameObjectmanager_->CreateInstance<Roller>("roller", BaseObject::TagMeteor);
	//隕石の座標
	roller->transform_.translate_.z = -10.0f;
	roller->transform_.translate_.x = -20.0f;
}

void RollerAtackState::Update()
{
	enemy_->ChangeState(std::make_unique<WaitTimeState>());
	return;
}

void RollerAtackState::DisplayImGui()
{
}
