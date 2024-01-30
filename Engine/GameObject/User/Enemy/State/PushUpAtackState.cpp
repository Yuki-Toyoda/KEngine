#include "PushUpAtackState.h"
#include "../IEnemy.h"
#include "../BossAnimManager.h"

void PushUpAtackState::Init()
{
	// �X�e�[�g���ݒ�
	name_ = "PushUp";
	enemy_->StateNumber_++;
}

	for (PushUp* pushUp : enemy_->pushUp_) {
		pushUp->SetActive();
	}

	// �˂��グ�U���A�j���[�V�����J�n
	enemy_->GetBossAnimManager()->PlayPushUpAttackAnim(enemy_->GetPushUpReadyTime());

}

void PushUpAtackState::Update()
{
	// �����U���I���A�j���[�V�����A�܂��̓_���[�W�A�j���[�V�������̏ꍇ
	if (enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_EndThrustUpAttack" ||
		enemy_->GetBossAnimManager()->GetAnimation()->GetReadingParameterName() == "Boss_Damage") {
	
		// �A�j���[�V�����i������芄���𒴂��Ă�����
		if (enemy_->GetBossAnimManager()->GetAnimation()->GetAnimationProgress() > 0.15f) {
			// �S�Ă̓˂��グ�U����U����Ԃɂ���
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
