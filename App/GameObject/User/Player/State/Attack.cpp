#include "Attack.h"
#include "Root.h"
#include "RotatingSlash.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void Attack::Init()
{
	stateName_ = "Attack";

	// 攻撃中である
	player_->isAttacking_ = true;

	// 攻撃アニメーションの再生
	player_->skiningModels_[0]->animationManager_.PlayAnimation("02_HorizontalSlash", 0.05f);

	// 線の座標を戻す
	player_->attackLine_->position_ = { 0.0f, 0.0f, 0.0f };

	// 素振りの効果音の再生
	Audio::GetInstance()->PlayWave(player_->SwingSword_);
}

void Attack::Update()
{
	if (player_->skiningModels_[0]->animationManager_.GetIsPlayingAnimation()) {

		// 6割り終わっていて、なおかつAボタン長押し中なら回転切りの準備に入る
		if (player_->skiningModels_[0]->animationManager_.GetPlayingAnimationProgress() >= 0.6f) {

			// 線の座標を戻す
			//player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };

			if (player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				// 行動を変更
				player_->ChangeState(std::make_unique<RotatingSlash>());

				// この先の処理を強制終了
				return;
			}
		}

		// 8割り終わっていたら再度攻撃可能
		if (player_->skiningModels_[0]->animationManager_.GetPlayingAnimationProgress() >= 0.8f) {
			
			// 線の座標を戻す
			//player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };

			if (player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				// 行動を変更
				player_->ChangeState(std::make_unique<Attack>());

				// この先の処理を強制終了
				return;
			}
		}
	}
	else {
		// 攻撃中でない
		player_->isAttacking_ = false;

		// プレイヤーのステートを再設定
		player_->ChangeState(std::make_unique<Root>());
	}
}

void Attack::DisplayImGui()
{

}
