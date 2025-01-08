#include "EnemyRoot.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void EnemyRoot::Init()
{
	// 行動名称の設定
	stateName_ = "Root";

	// 待機アニメーションの再生
	enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("Idle", 0.25f, true);
}

void EnemyRoot::Update()
{
	// 待機アニメーションが再生中でないとき
	if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() != "Idle") {
		// アニメーション終了まで待機して待機アニメーションを再生する
		if (!enemy_->skiningModels_["Enemy"]->animationManager_.GetIsPlayingAnimation()) {
			// 待機アニメーションの再生
			enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("Idle", 0.25f, true);
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
