#include "PushUpAtackState.h"
#include "../IEnemy.h"
#include "../BossAnimManager.h"

void PushUpAtackState::Init()
{
	// ステート名設定
	name_ = "PushUp";

	for (PushUp* pushUp : enemy_->pushUp_) {
		pushUp->SetActive();
	}

	// 突き上げ攻撃アニメーション開始
	enemy_->GetBossAnimManager()->PlayPushUpAttackAnim(enemy_->GetPushUpReadyTime());

}

void PushUpAtackState::Update()
{
	// 落下攻撃終了アニメーション、またはダメージアニメーション中の場合
	if (enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_EndThrustUpAttack" ||
		enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_Damage") {
	
		// アニメーション進捗が一定割合を超えていたら
		if (enemy_->GetBossAnimManager()->GetAnimation()->GetAnimationProgress() > 0.15f) {
			// 全ての突き上げ攻撃を攻撃状態にする
			for (PushUp* pushUp : enemy_->pushUp_) {
				pushUp->SetIsAttack(true);
			}

			enemy_->SetWaitTime(enemy_->GetWaitPushUp());
			enemy_->ChangeState(std::make_unique<WaitTimeState>());
			return;
		}
	}
}

void PushUpAtackState::DisplayImGui()
{
}
