#include "EnemyRoot.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void EnemyRoot::Init()
{
	// 行動名称の設定
	stateName_ = "Root";

	// ループを行う
	anim_->isLoop_ = true;
	// アニメーションを変更
	anim_->ChangeParameter("Enemy_Idle", 0.25f, true);
}

void EnemyRoot::Update()
{
	// アニメーションを変更
	if (anim_->GetReadingParameterName() != "Enemy_Idle") {
		// ループを切る
		if (anim_->isEnd_) {
			anim_->isLoop_ = true;
			anim_->ChangeParameter("Enemy_Idle", 0.15f, true);
		}
	}

	// 一定高度以上の場合
	if (enemy_->transform_.translate_.y >= kCanAttackHeight_) {
		// 攻撃可能状態に
		enemy_->SetCanAttack(true);
	}

	// 最高高度に達していない場合
	if (enemy_->transform_.translate_.y < kHeight_) {
		// 最高高度に線形補間を行う
		enemy_->transform_.translate_.y = KLib::Lerp<float>(enemy_->transform_.translate_.y, kHeight_, KLib::EaseOutQuad(klerpSpeed_));
	}
	else {
		// 超過している場合最高高度に補間を行う
		enemy_->transform_.translate_.y = kHeight_;
	}
}

void EnemyRoot::DisplayImGui()
{
}
