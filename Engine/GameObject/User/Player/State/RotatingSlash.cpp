#include "RotatingSlash.h"
#include "Attack.h"
#include "Root.h"
#include "../Player.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../LockOn/LockOn.h"
#include "../../Enemy/Enemy.h"

void RotatingSlash::Init()
{
	stateName_ = "RotatingSlash";

	// 攻撃中である
	player_->isAttacking_ = false;

	if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("03_RotateSlash_Charge")) {
		player_->skiningModels_[0]->animationManager_.PlayAnimation("03_RotateSlash_Charge", 0.1f, false);
	}

	// 線の座標を戻す
	player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };
}

void RotatingSlash::Update()
{
	// 回転斬りチャージアニメーションの再生
	if (player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("03_RotateSlash_Charge")) {
		// Aボタンを押し続けていないなら
		if (!(player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
			!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			// チャージが終わっていると
			if (isFinishedCharge_) {
				
				// 素振りの効果音の再生
				Audio::GetInstance()->PlayWave(player_->RotateSlash_);

				if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("05_RotateSlash")) {
					player_->skiningModels_[0]->animationManager_.PlayAnimation("05_RotateSlash");
				}

				// 回転斬り状態に
				isRotateSlashing_ = true;

				// 攻撃中でない
				player_->isAttacking_ = true;

				// 線の座標を戻す
				player_->attackLine_->position_ = { 0.0f, 0.0f, 0.0f };
			}
			else {
				// 攻撃中でない
				player_->isAttacking_ = false;

				// プレイヤーのステートを再設定
				player_->ChangeState(std::make_unique<Root>());

				// この先の処理を強制終了
				return;
			}
		}

		// アニメーション進捗が6割りを超えていれば
		if (player_->skiningModels_[0]->animationManager_.GetPlayingAnimationProgress() >= 0.6f) {
			// 回転切りチャージ完了
			isFinishedCharge_ = true;
		}
	}
	else if(!isRotateSlashing_) {
		// Aボタンを押し続けていないなら
		if (!(player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
			!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			// 素振りの効果音の再生
			Audio::GetInstance()->PlayWave(player_->RotateSlash_);

			if (!player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("05_RotateSlash")) {
				player_->skiningModels_[0]->animationManager_.PlayAnimation("05_RotateSlash");
			}

			// 回転斬り状態に
			isRotateSlashing_ = true;

			// 攻撃中でない
			player_->isAttacking_ = true;

			// 線の座標を戻す
			player_->attackLine_->position_ = { 0.0f, 0.0f, 0.0f };
		}
	}

	// 回転斬りアニメーションの再生
	if (player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation("05_RotateSlash")) {
		
	}
	else if(isRotateSlashing_){
		// 攻撃中でない
		player_->isAttacking_ = false;

		// プレイヤーのステートを再設定
		player_->ChangeState(std::make_unique<Root>());

		// この先の処理を強制終了
		return;
	}
}

void RotatingSlash::DisplayImGui()
{
}
