#include "SingleAtackState.h"
#include "../IEnemy.h"
void SingleAtackState::Init()
{
	name_ = "SingleAtack";
	//隕石を生成してstateを変更
	GameDataManager* jsonData = GameDataManager::GetInstance();
	// 参照の名前
	HierarchicalName names = { "SingleMeteor","Parameter"};
	names.kGroup = GameDataManager::GetInstance()->GetSingleAttack(0);
	int maxValue = jsonData->GetValue<int>(names, "MaxCount");

	for (int i = 0; i < maxValue; i++) {
		Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);
		//隕石の座標
		std::string paramName = "Meteor" + std::to_string(i);
		meteor->transform_.translate_ = jsonData->GetValue<Vector3>({ names.kGroup,paramName }, "Position");
		meteor->transform_.translate_.y = jsonData->GetValue<float>(names, "Distance");
		meteor->transform_.scale_ = jsonData->GetValue<Vector3>(names, "Scale");
		meteor->SetgameManager(enemy_->gameManager_);
	}
}

void SingleAtackState::Update()
{
	enemy_->SetWaitTime(enemy_->GetWaitSingle());
	enemy_->ChangeState(std::make_unique<WaitTimeState>());
	return;
}

void SingleAtackState::DisplayImGui()
{
}
