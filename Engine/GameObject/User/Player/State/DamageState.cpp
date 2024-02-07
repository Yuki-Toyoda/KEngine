#include "DamageState.h"
#include "../Player.h"
#include "../PlayerAnimManager.h"

void DamageState::Init()
{
	// 名前設定
	name_ = "Damage";
}

void DamageState::Update()
{
	// ダメージを喰らっていない状態であれば
	if (!player_->GetIsDamaged() && player_->pam_->GetAnimation()->GetReadingParameterName() != "Player_DamageRecovery") {
		// ステートを待機状態に変更
		player_->ChangeState(std::make_unique<RootState>());
		// 早期リターン
		return;
	}
}

void DamageState::DisplayImGui()
{
}
