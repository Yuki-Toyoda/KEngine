#include "SingleAtackState.h"
#include "../IEnemy.h"
void SingleAtackState::Init()
{
	name_ = "SingleAtack";
	//隕石を生成してstateを変更
	GameDataManager* jsonData = GameDataManager::GetInstance();
	// 参照の名前
	HierarchicalName names = { "MeteorParam","Info" };
	int maxValue = jsonData->GetValue<int>(names, "MaxCount");

	for (int i = 0; i < maxValue; i++) {
		Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);
		//隕石の座標
		std::string paramName = "Meteor" + std::to_string(i);
		meteor->transform_.translate_ = jsonData->GetValue<Vector3>({ "SingleMeteor",paramName }, "Position");
		meteor->transform_.translate_.y = jsonData->GetValue<float>({ "MeteorParam","Info" }, "Distance");
		meteor->transform_.scale_ = jsonData->GetValue<Vector3>(names, "Scale");
	}
}

void SingleAtackState::Update()
{

	enemy_->ChangeState(std::make_unique<EnemySampleState>());
	return;
}

void SingleAtackState::DisplayImGui()
{
}
