#include "RotatingSlash.h"
#include "Root.h"
#include "App/GameObject/User/Player/Player.h"

void RotatingSlash::Init()
{
	// ステート名の設定
	stateName_ = "RotatingSlash";

	// 攻撃中
	player_->GetAttackManager()->SetIsAttacking(true);
	// コライダー有効
	player_->GetSwordLine()->isActive_ = true;

	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("RotateSlash")) {
		player_->skiningModels_["Player"]->animationManager_.PlayAnimation("RotateSlash", 0.0f, false);
	}
}

void RotatingSlash::Update()
{
	// 回転斬りアニメーションの再生が終了している場合
	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("RotateSlash")) {
		// プレイヤーのステートを再設定
		player_->ChangeState(std::make_unique<Root>());

		// この先の処理を強制終了
		return;
	}
}

void RotatingSlash::DisplayImGui()
{
}
