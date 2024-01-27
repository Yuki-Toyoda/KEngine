#include "SingleAtackState.h"
#include "../IEnemy.h"
#include "../BossAnimManager.h"

void SingleAtackState::Init()
{
	name_ = "SingleAtack";
	//隕石を生成してstateを変更
	GameDataManager* jsonData = GameDataManager::GetInstance();
	// 参照の名前
	HierarchicalName names = { "SingleMeteor","Parameter"};
	names.kGroup = GameDataManager::GetInstance()->GetSingleAttack(enemy_->stateList_.stateNumber_[enemy_->patternNumber_][enemy_->StateNumber_]);
	int maxValue = jsonData->GetValue<int>(names, "MaxCount");

	for (int i = 0; i < maxValue; i++) {
		Meteor* meteor = gameObjectmanager_->CreateInstance<Meteor>("Meteor", BaseObject::TagMeteor);
		//隕石の座標
		std::string paramName = "Meteor" + std::to_string(i);
		meteor->transform_.translate_ = jsonData->GetValue<Vector3>({ names.kGroup,paramName }, "Position");
		meteor->transform_.translate_.y = jsonData->GetValue<float>(names, "Distance");
		meteor->transform_.scale_ = jsonData->GetValue<Vector3>(names, "Scale");
		meteor->SetgameManager(enemy_->gameManager_);

		// 野菜配列に追加
		meteors_.push_back(meteor);
	}

	// 落下攻撃アニメーション開始
	enemy_->GetBossAnimManager()->PlayFallAttackAnim(enemy_->GetFallAttackReadyTime());

}

void SingleAtackState::Update()
{
	// 落下攻撃終了アニメーション、またはダメージアニメーション中の場合
	if (enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_EndFallAttack" ||
		enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_Damage") {
		
		// 生成した全ての野菜の落下トリガーをtrue
		for (Meteor* m : meteors_) {
			m->SetIsFall(true);
		}

		// 敵の攻撃後待機時間を設定
		enemy_->SetWaitTime(enemy_->GetWaitSingle());
		// 敵の行動状態を待機中状態に変更
		enemy_->ChangeState(std::make_unique<WaitTimeState>());

		// その後の処理を行わない
		return;
	}	
}

void SingleAtackState::DisplayImGui()
{
}
