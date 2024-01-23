#include "SingleAtackState.h"
#include "../IEnemy.h"
void SingleAtackState::Init()
{
	name_ = "SingleAtack";
	//隕石を生成してstateを変更
	Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("meteor", BaseObject::TagMeteor);
	//隕石の座標
	meteor->transform_.translate_.y = 30.0f;
	meteor->transform_.translate_.z = -10.0f;
}

void SingleAtackState::Update()
{
	
	enemy_->ChangeState(std::make_unique<WaitTimeState>());
	return;
}

void SingleAtackState::DisplayImGui()
{
}
