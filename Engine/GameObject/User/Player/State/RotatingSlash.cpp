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

	if (!player_->transform_.animations_[3].isPlay) {
		player_->transform_.animations_[3].isPlay = true;
		player_->transform_.animations_[3].animationTime = 0.0f;
		player_->transform_.animations_[0].isPlay = false;
		player_->transform_.animations_[1].isPlay = false;
		player_->transform_.animations_[2].isPlay = false;
	}

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
				
				// 素振りの効果音の再生
				Audio::GetInstance()->PlayWave(player_->RotateSlash_);

				if (!player_->transform_.animations_[4].isPlay) {
					player_->transform_.animations_[4].isPlay = true;
					player_->transform_.animations_[4].animationTime = 0.0f;
					player_->transform_.animations_[0].isPlay = false;
					player_->transform_.animations_[1].isPlay = false;
					player_->transform_.animations_[2].isPlay = false;
					player_->transform_.animations_[3].isPlay = false;
				}

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

				player_->transform_.animations_[0].isPlay = false;
				player_->transform_.animations_[1].isPlay = false;
				player_->transform_.animations_[2].isPlay = false;
				player_->transform_.animations_[3].isPlay = false;
				player_->transform_.animations_[4].isPlay = false;

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

			// 素振りの効果音の再生
			Audio::GetInstance()->PlayWave(player_->RotateSlash_);

			// プレイヤーのアニメーションの変更
			player_->playerAnim_->ChangeParameter("Player_RotaingSlash", true);

			// アニメーションのループを無効
			player_->playerAnim_->isLoop_ = false;

			if (!player_->transform_.animations_[4].isPlay) {
				player_->transform_.animations_[4].isPlay = true;
				player_->transform_.animations_[4].animationTime = 0.0f;
				player_->transform_.animations_[0].isPlay = false;
				player_->transform_.animations_[1].isPlay = false;
				player_->transform_.animations_[2].isPlay = false;
				player_->transform_.animations_[3].isPlay = false;
			}

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

			player_->transform_.animations_[0].isPlay = false;
			player_->transform_.animations_[1].isPlay = false;
			player_->transform_.animations_[2].isPlay = false;
			player_->transform_.animations_[3].isPlay = false;
			player_->transform_.animations_[4].isPlay = false;

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
