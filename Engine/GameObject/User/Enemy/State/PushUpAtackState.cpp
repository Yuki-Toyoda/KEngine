#include "PushUpAtackState.h"
#include "../IEnemy.h"
#include "../BossAnimManager.h"

void PushUpAtackState::Init()
{
	// ステート名設定
	name_ = "PushUp";
	// ステージデータ
	GameDataManager* dataManager = GameDataManager::GetInstance();
	
	// リスト初期化
	enemy_->pushUp_.clear();

	// グループ名作成
	std::string group = dataManager->GetPushUpAttack(enemy_->stateList_.stateNumber_[enemy_->patternNumber_][enemy_->StateNumber_]);

	int pushUpMax = dataManager->GetValue<int>({ group,"Parameter" }, "MaxCount");
	for (int i = 0; i < pushUpMax; i++) {
		// インスタンス生成
		PushUp* pushUp;
		pushUp = GameObjectManager::GetInstance()->CreateInstance<PushUp>("PushUp", BaseObject::TagNone);
		// 名前
		std::string name = "PushUp" + std::to_string(i);
		// Y座標以外を設定
		Vector3 newPos = dataManager->GetValue<Vector3>({ group,name }, "Position");
		pushUp->transform_.translate_.x = newPos.x;
		pushUp->transform_.translate_.z = newPos.z;
		enemy_->pushUp_.push_back(pushUp);
	}


	for (PushUp* pushUp : enemy_->pushUp_) {
		pushUp->SetActive();
	}

	enemy_->StateNumber_++;

	// 突き上げ攻撃アニメーションを再生する
	enemy_->GetBossAnimManager()->PlayPushUpAttackAnim(enemy_->GetPushUpReadyTime());

}

void PushUpAtackState::Update()
{
	// ボスの攻撃モーションの終了時、またはダメージモーション終了時に
	if (enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_EndThrustUpAttack" ||
		enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_Damage") {
	
		// アニメーション進捗が一定値以上になったら突き上げ攻撃を有効にする
		if (enemy_->GetBossAnimManager()->GetAnimation()->GetAnimationProgress() > 0.15f) {
			// 突き上げ攻撃を行う
			for (PushUp* pushUp : enemy_->pushUp_) {
				pushUp->SetIsAttack(true);
			}

			// ステート変更
			enemy_->SetWaitTime(enemy_->GetWaitPushUp());
			enemy_->ChangeState(std::make_unique<WaitTimeState>());
			return;
		}
	}
}

void PushUpAtackState::DisplayImGui()
{
}
