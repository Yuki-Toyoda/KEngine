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

	// プレイヤーのアニメーションの変更
	player_->playerAnim_->ChangeParameter("Player_RotaingSlashCharge", true);
	// アニメーションのループを無効
	player_->playerAnim_->isLoop_ = false;
	// 攻撃中である
	player_->isAttacking_ = true;

	// 線の座標を戻す
	player_->attackLine_->position_ = { -1000.0f, 100.0f, 0.0f };
}

void RotatingSlash::Update()
{
	// 回転斬りチャージアニメーションの再生
	if (player_->playerAnim_->GetReadingParameterName() == "Player_RotaingSlashCharge") {
		// アニメーションが終了していれば
		if (player_->playerAnim_->isEnd_ == true) {
			// 攻撃中でない
			player_->isAttacking_ = false;

			// プレイヤーのアニメーションの変更
			player_->playerAnim_->ChangeParameter("Player_RotaingSlashChargeing", true);

			// アニメーションのループを無効
			player_->playerAnim_->isLoop_ = true;
		}

		// Aボタンを押し続けていないなら
		if (!(player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
			!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			// アニメーション進捗が6割りを超えていれば
			if (player_->playerAnim_->GetAnimationProgress() >= 0.6f) {
				// 攻撃中でない
				player_->isAttacking_ = true;

				// プレイヤーのアニメーションの変更
				player_->playerAnim_->ChangeParameter("Player_RotaingSlash", true);

				// アニメーションのループを無効
				player_->playerAnim_->isLoop_ = false;

				// 線の座標を戻す
				player_->attackLine_->position_ = { -0.7f, 0.0f, 0.0f };
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
	}

	// 回転斬りチャージ中アニメーションの再生
	if (player_->playerAnim_->GetReadingParameterName() == "Player_RotaingSlashChargeing") {
		// Aボタンを離したら
		if (!(player_->joyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A) &&
			!(player_->preJoyState_.Gamepad.wButtons & XINPUT_GAMEPAD_A)) {
			// 攻撃中でない
			player_->isAttacking_ = true;

			// プレイヤーのアニメーションの変更
			player_->playerAnim_->ChangeParameter("Player_RotaingSlash", true);

			// アニメーションのループを無効
			player_->playerAnim_->isLoop_ = false;

			// 線の座標を戻す
			player_->attackLine_->position_ = { -0.7f, 0.0f, 0.0f };
		}
	}

	// 回転斬りアニメーションの再生
	if (player_->playerAnim_->GetReadingParameterName() == "Player_RotaingSlash") {
		// アニメーションが終了していれば
		if (player_->playerAnim_->isEnd_ == true) {
			// 攻撃中でない
			player_->isAttacking_ = false;

			// プレイヤーのステートを再設定
			player_->ChangeState(std::make_unique<Root>());

			// この先の処理を強制終了
			return;
		}
	}
}

void RotatingSlash::DisplayImGui()
{
}
