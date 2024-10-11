#include "Attack.h"
#include "Root.h"
#include "RotatingSlash.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void Attack::Init()
{
	// ステート名を設定
	stateName_ = "Attack";

	// 攻撃中である
	player_->isAttacking_ = true;
	// コライダー有効
	for (const auto& collider : player_->colliders_) {
		// 剣のコライダーを発見した場合
		if (collider->GetColliderName() == "Sword") {
			collider->SetIsActive(true);
		}
	}

	// 攻撃アニメーションの再生
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation("08_HorizontalSlash", 0.05f);

	// 素振りの効果音の再生
	Audio::GetInstance()->PlayWave(player_->SwingSword_);
}

void Attack::Update()
{
	// アニメーション再生中の場合
	if (player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation()) {

		// アニメーションの6割が終了している場合
		if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() >= 0.6f) {
			// Aボタンが長押しされていれば
			if (player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				// 回転斬りステートへ
				player_->ChangeState(std::make_unique<RotatingSlash>());

				// この先の処理を強制終了
				return;
			}
		}

		// アニメーションの8割が終了している場合
		if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() >= 0.8f) {
			// Aボタンを彩度トリガーした場合
			if (player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				// もう一度攻撃ステートへ
				player_->ChangeState(std::make_unique<Attack>());

				// この先の処理を強制終了
				return;
			}
		}
	}
	else {
		// 攻撃中でない
		player_->isAttacking_ = false;

		// プレイヤーのステートを待機状態へ
		player_->ChangeState(std::make_unique<Root>());
	}
}

void Attack::DisplayImGui()
{

}
