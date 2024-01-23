#include "RollerAtackState.h"
#include "../IEnemy.h"
void RollerAtackState::Init()
{
	name_ = "Roller";
	////隕石を生成してstateを変更
	//Roller* roller = gameObjectmanager_->CreateInstance<Roller>("roller", BaseObject::TagMeteor);
	////隕石の座標
	//roller->transform_.translate_.z = -10.0f;
	//roller->transform_.translate_.x = -20.0f;

	GameDataManager* dataManager = GameDataManager::GetInstance();

	int maxCount = dataManager->GetValue<int>({ "AttackParam","Roller" }, "MaxCount");
	for (int i = 0; i < maxCount; i++) {
		Roller* object = gameObjectmanager_->CreateInstance<Roller>("Roller", BaseObject::TagMeteor);

		std::string group = "RollerAttack";
		std::string section = "Roller" + std::to_string(i);

		Vector3 newPos = dataManager->GetValue<Vector3>({ group,section }, "Position");
		object->transform_.translate_.z = newPos.z;
		object->transform_.translate_.x = newPos.x;
	}

}

void RollerAtackState::Update()
{
	enemy_->SetWaitTime(enemy_->GetWaitRoller());
	enemy_->ChangeState(std::make_unique<WaitTimeState>());
	return;
}

void RollerAtackState::DisplayImGui()
{
}
