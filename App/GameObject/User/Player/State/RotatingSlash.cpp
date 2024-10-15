#include "RotatingSlash.h"
#include "Attack.h"
#include "Root.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void RotatingSlash::Init()
{
	// ステート名の設定
	stateName_ = "RotatingSlash";

	// 攻撃中である
	player_->isAttacking_ = false;
	// コライダー無効
	for (const auto& collider : player_->colliders_) {
		// 剣のコライダーを発見した場合
		if (collider->GetColliderName() == "Sword") {
			collider->SetIsActive(false);
		}
	}

	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("12_RotateSlash_Charge")) {
		player_->skiningModels_["Player"]->animationManager_.PlayAnimation("12_RotateSlash_Charge", 0.1f, false);
	}
}

void RotatingSlash::Update()
{
	// 回転斬りチャージアニメーションの再生中であれば
	if (player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("12_RotateSlash_Charge")) {
		// Aボタンを押し続けていないなら
		if (input_->InspectButton(XINPUT_GAMEPAD_A, RELEASE)) {
			// チャージが終わっていると
			if (isFinishedCharge_) {
				
				// 素振りの効果音の再生
				Audio::GetInstance()->PlayWave(player_->RotateSlash_);

				if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("14_RotateSlash")) {
					player_->skiningModels_["Player"]->animationManager_.PlayAnimation("14_RotateSlash");
				}

				// 回転斬り状態に
				isRotateSlashing_ = true;

				// 攻撃中でない
				player_->isAttacking_ = true;

				// コライダー有効
				for (const auto& collider : player_->colliders_) {
					// 剣のコライダーを発見した場合
					if (collider->GetColliderName() == "Sword") {
						collider->SetIsActive(true);
					}
				}
			}
			else {
				// 攻撃中でない
				player_->isAttacking_ = false;

				// コライダー無効
				for (const auto& collider : player_->colliders_) {
					// 剣のコライダーを発見した場合
					if (collider->GetColliderName() == "Sword") {
						collider->SetIsActive(false);
					}
				}

				// プレイヤーのステートを再設定
				player_->ChangeState(std::make_unique<Root>());

				// この先の処理を強制終了
				return;
			}
		}

		// アニメーション進捗が6割りを超えていれば
		if (player_->skiningModels_["Player"]->animationManager_.GetPlayingAnimationProgress() >= 0.6f) {
			// 回転切りチャージ完了
			isFinishedCharge_ = true;
		}
	}
	else if(!isRotateSlashing_) {
		// Aボタンを押し続けていないなら
		if (input_->InspectButton(XINPUT_GAMEPAD_A, RELEASE)) {
			// 素振りの効果音の再生
			Audio::GetInstance()->PlayWave(player_->RotateSlash_);

			if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("14_RotateSlash")) {
				player_->skiningModels_["Player"]->animationManager_.PlayAnimation("14_RotateSlash");
			}

			// 回転斬り状態に
			isRotateSlashing_ = true;

			// 攻撃中でない
			player_->isAttacking_ = true;

			// コライダー有効
			for (const auto& collider : player_->colliders_) {
				// 剣のコライダーを発見した場合
				if (collider->GetColliderName() == "Sword") {
					collider->SetIsActive(true);
				}
			}
		}
	}

	// 回転斬りアニメーションの再生
	if (player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("14_RotateSlash")) {
		
	}
	else if(isRotateSlashing_){
		// 攻撃中でない
		player_->isAttacking_ = false;

		// コライダー無効
		for (const auto& collider : player_->colliders_) {
			// 剣のコライダーを発見した場合
			if (collider->GetColliderName() == "Sword") {
				collider->SetIsActive(false);
			}
		}

		// プレイヤーのステートを再設定
		player_->ChangeState(std::make_unique<Root>());

		// この先の処理を強制終了
		return;
	}
}

void RotatingSlash::DisplayImGui()
{
}
