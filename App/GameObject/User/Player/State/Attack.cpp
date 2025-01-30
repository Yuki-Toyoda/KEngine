#include "Attack.h"
#include "Root.h"
#include "App/GameObject/User/Player/Player.h"
#include "App/GameObject/User/FollowCamera/FollowCamera.h"
#include "App/GameObject/User/LockOn/LockOn.h"
#include "App/GameObject/User/Enemy/Enemy.h"

void Attack::Init()
{
	// ステート名を設定
	stateName_ = "Attack";

	// コライダー有効
	player_->GetSwordLine()->GetCollider()->SetIsActive(true);

	// 素振りの効果音の再生
	Audio::GetInstance()->PlayWave(player_->SwingSword_);

	// コンボマネージャー取得
	comboManager_ = player_->GetComboManager();
}

void Attack::Update()
{
	// コンボマネージャー更新
	comboManager_->Update();

	// コンボ終了時に待機状態に移行する
	if (comboManager_->GetIsComboEnd()) {
		// 攻撃中でない
		player_->SetIsAttacking(false);

		// プレイヤーのステートを待機状態へ
		player_->ChangeState(std::make_unique<Root>());
	}
}

void Attack::DisplayImGui()
{

}
