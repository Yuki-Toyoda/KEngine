#include "SingleAtackState.h"
#include "../IEnemy.h"
void SingleAtackState::Init()
{
	name_ = "SingleAtack";
}

void SingleAtackState::Update()
{
	////隕石を生成してstateを変更
	//Meteor* meteor=gameObjectmanager_->CreateInstance<Meteor>("meteor", BaseObject::TagMeteor);
	////隕石の座標
	//meteor->transform_.translate_.y = 30.0f;
	//meteor->transform_.translate_.z = -10.0f;
	//隕石を生成してstateを変更
	GameDataManager* jsonData = GameDataManager::GetInstance();
	int maxValue = jsonData->GetIntValue({ "Boss","A" }, "MaxCount");

	for (int i = 0; i < maxValue; i++) {
		Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);
		//隕石の座標
		std::string paramName = "Meteor" + std::to_string(i);
		meteor->transform_.translate_ = jsonData->GetVector3Value({ "Boss",paramName }, "Position");
		meteor->transform_.translate_.y = 30.0f;
	}

	enemy_->ChangeState(std::make_unique<EnemySampleState>());
	return;
}

void SingleAtackState::DisplayImGui()
{
}
