#include "EnemyRoot.h"
#include "../Enemy.h"

void EnemyRoot::Init()
{
	// 行動名称の設定
	stateName_ = "Root";

	// ループを行う
	enemy_->enemyAnim_->isLoop_ = true;
	// アニメーションを変更
	enemy_->enemyAnim_->ChangeParameter("Enemy_Idle", 0.25f, true);
}

void EnemyRoot::Update()
{
	// アニメーションを変更
	if (enemy_->enemyAnim_->GetReadingParameterName() != "Enemy_Idle") {
		// ループを切る
		if (enemy_->enemyAnim_->isEnd_) {
			enemy_->enemyAnim_->isLoop_ = true;
			enemy_->enemyAnim_->ChangeParameter("Enemy_Idle", 0.15f, true);
		}
	}

	// 最高高度に達していない場合
	if (enemy_->transform_.translate_.y < kHeight_) {
		// 最高高度に線形補間を行う
		enemy_->transform_.translate_.y = Math::EaseOut(klerpSpeed_, enemy_->transform_.translate_.y, kHeight_);
	}
	else {
		// 超過している場合最高高度に補間を行う
		enemy_->transform_.translate_.y = kHeight_;
	}
}

void EnemyRoot::DisplayImGui()
{
}
