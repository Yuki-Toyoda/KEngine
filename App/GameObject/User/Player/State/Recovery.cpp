#include "Recovery.h"
#include "Root.h"
#include "App/GameObject/User/Player/Player.h"

void Recovery::Init()
{
	// ステート名の設定
	stateName_ = "Recovery";

	// 復帰アニメーションの再生
	player_->skiningModels_["Player"]->animationManager_.PlayAnimation("Recovery");
}

void Recovery::Update()
{
	// 復帰アニメーションが終了している場合
	if (!player_->skiningModels_["Player"]->animationManager_.GetIsPlayingAnimation("Recovery")) {
		// 待機状態に移行
		player_->ChangeState(std::make_unique<Root>());
		// 以降の処理を無視
		return;
	}
}

void Recovery::DisplayImGui()
{
}
