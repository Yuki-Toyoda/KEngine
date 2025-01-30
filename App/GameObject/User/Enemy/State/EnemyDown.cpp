#include "EnemyDown.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "EnemyRoot.h"

void EnemyDown::Init()
{
	// 行動名設定
	stateName_ = "Down";

	// 近接攻撃状態解除
	enemy_->SetIsCQCAttack(false);

	// ダウン開始アニメーションを再生
	enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("DownStart");
	enemy_->skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(1.0f, "DownStart");

	// ダウン時間タイマースタート
	timer_.Start(kMaxDownTime_);

	// 回転固定解除
	enemy_->SetIsRotateLock(true);
}

void EnemyDown::Update()
{
	// 線形補間で落下させる
	if (enemy_->transform_.translate_.y > kHeight_) {
		// 最低高度に線形補間を行う
		enemy_->transform_.translate_.y = KLib::Lerp<float>(enemy_->transform_.translate_.y, kHeight_, KLib::EaseOutQuad(klerpSpeed_));
	}
	else {
		// 超過している場合最低高度に補間を行う
		enemy_->transform_.translate_.y = kHeight_;
	}

	// 再生中アニメーションがダウン、またはダメージなら
	if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "DownStart" || 
		enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "Damage") {
		// 終了していたら次のアニメーションに移る
		if (!enemy_->skiningModels_["Enemy"]->animationManager_.GetIsPlayingAnimation()) {
			// ダウン中アニメーションを再生
			enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("DownWaiting", 0.15f, true);
			enemy_->skiningModels_["Enemy"]->animationManager_.SetAnimationSpeed(1.0f, "DownWaiting");
		}

	}

	// 起き上がらない状態であれば早期リターン
	if (enemy_->GetIsNeverDown()) { return; }

	// 再生中のパラメータがダウン中アニメーション中なら
	if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "DownWaiting" ||
		enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "Damage") {
		// タイマーが終了時
		if (timer_.GetIsFinish()) {
			// 麻痺パーティクルのインスタンスが存在する場合
			if (enemy_->GetStunParticle() != nullptr) {
				// 強制敵に停止
				enemy_->GetStunParticle()->SetIsEnd(true);
				enemy_->ResetStunParticle();
			}

			// 待機状態に移行
			enemy_->ChangeState(std::make_unique<EnemyRoot>());
			// これ以降の処理を無視
			return;
		}

		// タイマーの更新
		timer_.Update();
	}
}

void EnemyDown::DisplayImGui()
{
}
