#include "RotatingSlashCharge.h"
#include "App/GameObject/User/Player/Player.h"
#include "RotatingSlash.h"
#include "Root.h"

void RotatingSlashCharge::Init()
{
	// ステート名の設定
	stateName_ = "RotatingSlashCharge";

	// 攻撃中でない
	player_->SetIsAttacking(false);
	// コライダー無効
	player_->GetSwordLine()->isActive_ = false;

	// チャージアニメーションの再生
	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("12_RotateSlash_Charge")) {
		player_->skiningModels_["Player"]->animationManager_.PlayAnimation("12_RotateSlash_Charge", 0.1f, false);
	}
}

void RotatingSlashCharge::Update()
{
	// Aボタンを離した場合
	if (input_->InspectButton(XINPUT_GAMEPAD_A, RELEASE)) {
		// チャージが終了していないかで処理を分岐
		if (isFinishedCharge_) { // チャージが終了している場合回転斬りを行う
			// プレイヤーのステートを再設定
			player_->ChangeState(std::make_unique<RotatingSlash>());
			// この先の処理を強制終了
			return;
		}
		else { // そうでない場合は待機状態へ移行
			// プレイヤーのステートを再設定
			player_->ChangeState(std::make_unique<Root>());
			// この先の処理を強制終了
			return;
		}
	}

	// アニメーション進捗が6割りを超えていれば
	if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() >= chargeTimeThreshold_) {
		// 回転切りチャージ完了
		isFinishedCharge_ = true;
	}
}

void RotatingSlashCharge::DisplayImGui()
{
}
