#include "MultiAtackState.h"
#include "../IEnemy.h"

void MultiAtackState::Init()
{
	name_ = "MultiAtack";
	GameDataManager* dataManager = GameDataManager::GetInstance();
	coolTime_ = dataManager->GetValue<float>({ "AttackParam","Multi" }, "CoolTime");
	kMaxCount_ = dataManager->GetValue<int>({ "AttackParam","Multi" }, "MaxCount");

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
		meteor->transform_.translate_.y = 30.0f;
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

