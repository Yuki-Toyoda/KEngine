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

	// 突き上げ攻撃アニメーションを再生する
	enemy_->GetBossAnimManager()->PlayPushUpAttackAnim(enemy_->GetPushUpReadyTime());
	enemy_->StateNumber_++;
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
