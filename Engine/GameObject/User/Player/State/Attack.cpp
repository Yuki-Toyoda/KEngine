#include "Attack.h"
#include "Root.h"
#include "../Player.h"
#include "../../FollowCamera/FollowCamera.h"
#include "../../LockOn/LockOn.h"
#include "../../Enemy/Enemy.h"

void Attack::Init()
{
	// プレイヤーのアニメーションの変更
	player_->playerAnim_->ChangeParameter("Player_HorizontalSlash", true);
	// アニメーションのループを無効
	player_->playerAnim_->isLoop_ = false;
	// 攻撃中である
	player_->isAttacking_ = true;

	// 線の座標を戻す
	player_->attackLine_->position_ = { -0.7f, 0.0f, 0.0f };
}

void Attack::Update()
{
	if (!player_->playerAnim_->isEnd_) {

		// 8割り終わっていたら再度攻撃可能
		if (player_->playerAnim_->GetAnimationProgress() >= 0.8f) {
			if (player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A &&
				!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
				// 行動を変更
				player_->ChangeState(std::make_unique<Attack>());
			}

			// 線の座標を戻す
			//player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };
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
