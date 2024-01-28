#include "RollerAtackState.h"
#include "../IEnemy.h"
#include "../BossAnimManager.h"

void RollerAtackState::Init()
{
	name_ = "Roller";

	GameDataManager* dataManager = GameDataManager::GetInstance();
	std::string group = dataManager->GetRollerAttack(enemy_->stateList_.stateNumber_[enemy_->patternNumber_][enemy_->StateNumber_]);
	int maxCount = dataManager->GetValue<int>({ group,"Parameter"}, "MaxCount");
	for (int i = 0; i < maxCount; i++) {
		Roller* object = gameObjectmanager_->CreateInstance<Roller>("Roller", BaseObject::TagMeteor);

		//std::string group = "RollerAttack";
		std::string section = "Roller" + std::to_string(i);

		Vector3 newPos = dataManager->GetValue<Vector3>({ group,section }, "Position");
		object->transform_.translate_.z = newPos.z;
		object->transform_.translate_.x = newPos.x;
		object->SetVelocity(dataManager->GetValue<Vector3>({ group,section }, "Direct"));
		object->transform_.scale_ = dataManager->GetValue<Vector3>({ group,"Parameter" }, "Scale");
		object->SetgameManager(enemy_->gameManager_);

		// 一時格納のため配列に保存
		rollers_.push_back(object);
	}
	
	// 落下攻撃アニメーション開始
	enemy_->GetBossAnimManager()->PlayRollerAttackAnim(enemy_->GetRollerAttackReadyTime());

}

void RollerAtackState::Update()
{
	// 落下攻撃終了アニメーションの進捗が一定値以上、またはダメージアニメーション中の場合
	if ((enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_EndRollerAttack" && 
		enemy_->GetBossAnimManager()->GetAnimation()->GetAnimationProgress() >= 0.25f) ||
		enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_Damage") {

		// 全てのローラーを同タイミングで動作させる
		for (Roller* r : rollers_) {
			r->SetIsMove(true);
		}

		// ステート変更
		enemy_->SetWaitTime(enemy_->GetWaitRoller());
		enemy_->ChangeState(std::make_unique<WaitTimeState>());
		// これ以降の処理を行わない
		return;
	}
}

void RollerAtackState::DisplayImGui()
{
}
