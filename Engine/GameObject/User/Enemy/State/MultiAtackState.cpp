#include "MultiAtackState.h"
#include "../IEnemy.h"

void MultiAtackState::Init()
{
	name_ = "MultiAtack";
	GameDataManager* dataManager = GameDataManager::GetInstance();
	std::string group = GameDataManager::GetInstance()->GetMultiAttack(0);
	coolTime_ = dataManager->GetValue<float>({ group,"Parameter" }, "CoolTime");
	kMaxCount_ = dataManager->GetValue<int>({ group,"Parameter" }, "MaxCount");

	atackTimer_.Start(coolTime_);
}

void MultiAtackState::Update()
{
	atackTimer_.Update();
	if (atackTimer_.GetIsFinish()) {
		//攻撃回数を増加
		atackCount_++;
		//タイマーをスタート
		atackTimer_.Start(coolTime_);
		//隕石を生成して
		Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("meteor", BaseObject::TagMeteor);
		//隕石の座標
		meteor->transform_.translate_ = enemy_->GetPlayer()->transform_.translate_;
		GameDataManager* dataManager = GameDataManager::GetInstance();
		std::string group = dataManager->GetMultiAttack(0);
		std::string distance = dataManager->GetSingleAttack(0);
		meteor->transform_.translate_.y = dataManager->GetValue<float>({ distance,"Parameter" }, "Distance");
		meteor->transform_.scale_ = dataManager->GetValue<Vector3>({ group,"Parameter" }, "Scale");
		meteor->SetgameManager(enemy_->gameManager_);
	}
	if (atackCount_ >= kMaxCount_) {
		//stateを変更
		enemy_->SetWaitTime(enemy_->GetWaitMulti());
		enemy_->ChangeState(std::make_unique<WaitTimeState>());
		return;
	}
}

void MultiAtackState::DisplayImGui()
{
}

