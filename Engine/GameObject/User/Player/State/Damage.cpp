#include "Damage.h"
#include "Root.h"
#include "../Player.h"

void Damage::Init()
{
	stateName_ = "Damage";

	// アニメーションのループを切る
	player_->playerAnim_->isLoop_ = false;

	// プレイヤーアニメーションを変更
	player_->playerAnim_->ChangeParameter("Player_Damage", true);
	
	// 攻撃中でない
	player_->isAttacking_ = false;

	// 速度を求める
	velocity_ = { 0.0f, 0.0f, -0.1f };
	Matrix4x4 rotateMat = Math::MakeRotateYMatrix(player_->transform_.rotate_.y);
	velocity_ = Math::Transform(velocity_, rotateMat);
}

void Damage::Update()
{
	// ダメージアニメーション中なら
	if (player_->playerAnim_->GetReadingParameterName() == "Player_Damage") {
		// アニメーションの5割が終了するまで、後ろに吹っ飛ばす
		if (player_->playerAnim_->GetAnimationProgress() <= 0.25f) {
			player_->transform_.translate_ += velocity_;
		}
		
		// アニメーションの9割りが終了していた場合、スティックの入力があった場合待機状態へ移行
		if (player_->playerAnim_->GetAnimationProgress() >= 0.9f) {
			
		}

		// アニメーションが終了していれば
		if (player_->playerAnim_->isEnd_) {
			// 待機状態に移行
			player_->ChangeState(std::make_unique<Root>());
		}
	}
}

void Damage::DisplayImGui()
{
}
