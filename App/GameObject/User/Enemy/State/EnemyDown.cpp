#include "EnemyDown.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "EnemyRoot.h"

void EnemyDown::Init()
{
	// 行動名設定
	stateName_ = "Down";

	// ループを切れる
	anim_->isLoop_ = false;
	// 敵のアニメーションを変更
	anim_->ChangeParameter("Enemy_Down", true);

	// ダウン時間タイマースタート
	timer_.Start(kMaxDownTime_);
}

void EnemyDown::Update()
{
	// 最低高度に達していない場合
	if (isFalling_) {
		if (enemy_->transform_.translate_.y > kHeight_) {
			// 最低高度に線形補間を行う
			enemy_->transform_.translate_.y = KLib::Lerp<float>(enemy_->transform_.translate_.y, kHeight_, KLib::EaseOutQuad(klerpSpeed_));
		}
		else {
			// 超過している場合最低高度に補間を行う
			enemy_->transform_.translate_.y = kHeight_;
		}
	}

	// 再生中のパラメータがダウンアニメーション中なら
	if (anim_->GetReadingParameterName() == "Enemy_Down" || anim_->GetReadingParameterName() == "Enemy_Damage") {
		if (anim_->GetAnimationProgress() >= 0.35f) {
			isFalling_ = true;
		}

		// 終了していたら次のアニメーションに移る
		if (anim_->isEnd_) {
			// ループを行う
			anim_->isLoop_ = true;
			// 敵のアニメーションを変更
			anim_->ChangeParameter("Enemy_Downing", true);
		}
	}

	// 再生中のパラメータがダウン中アニメーション中なら
	if (anim_->GetReadingParameterName() == "Enemy_Downing" || anim_->GetReadingParameterName() == "Enemy_Damage") {
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

		if (anim_->GetReadingParameterName() == "Enemy_Damage") {// ループを行う
			if (anim_->isEnd_) {
				anim_->isLoop_ = true;
				// 敵のアニメーションを変更
				anim_->ChangeParameter("Enemy_Downing", true);
			}
		}

		// タイマーの更新
		timer_.Update();
	}
}

void EnemyDown::DisplayImGui()
{
}
