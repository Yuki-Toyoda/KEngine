#include "EnemyShot.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/Enemy/EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "Engine/GameObject/GameObjectManager.h"

void EnemyShot::Init()
{
	// 行動名設定
	stateName_ = "Shot";

	// 射撃開始アニメーションの再生
	enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("ShotStart", 0.1f, false);

	// タイマーの初期化
	timer_.Start(KLib::RandomF(kMinShotTime_, kMaxShotTime_, 2));

	// 回転固定解除
	enemy_->SetIsRotateLock(false);
}

void EnemyShot::Update()
{
	// チャージアニメーションの場合
	if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "ShotStart") {
		// アニメーション進捗が6割以上進んでいるかつ、弾が生成されていない場合
		if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationProgress() >= 0.6f && 
			bullet_ == nullptr) {
			// 弾を生成する
			bullet_ = GameObjectManager::GetInstance()->CreateInstance<EnemyBullet>("EnemyBullet", IObject::TagEnemy);

			// プレイヤーの座標をセット
			Player* player = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
			bullet_->SetPlayerTransform(player->GetBodyPos());
			// 敵の座標をセット
			bullet_->SetEnemyTransform(enemy_->GetBodyTransform());
			// 敵の左腕行列の取得
			leftArmMatrix_ = enemy_->skiningModels_["Enemy"]->GetBoneMatrix("Hand.L");
			// 左腕行列を用いて弾を左腕にペアレントする
			bullet_->ParentPosition(&leftArmMatrix_, &enemy_->transform_);
		}
		
		// アニメーションが終了しているなら
		if (!enemy_->skiningModels_["Enemy"]->animationManager_.GetIsPlayingAnimation()) {
			// 次のアニメーションを設定
			enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("ShotWaiting", 0.0f, true);
		}

		// 敵の左腕行列の更新
		leftArmMatrix_ = enemy_->skiningModels_["Enemy"]->GetBoneMatrix("Hand.L");
	}

	// チャージ中アニメーションの場合
	if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "ShotWaiting") {
		// タイマー終了時
		if (timer_.GetIsFinish()) {
			// 次のアニメーションを設定
			enemy_->skiningModels_["Enemy"]->animationManager_.PlayAnimation("Shot", 0.1f, false);
			// プレイヤーに向かって弾を発射する
			bullet_->SetVelocity(true, enemy_->GetRallyCount());

			// 弾と左腕の親子付けを解除する
			bullet_->UnParent();

			// その後の処理を強制終了
			return;
		}

		// 敵の左腕行列の取得
		leftArmMatrix_ = enemy_->skiningModels_["Enemy"]->GetBoneMatrix("Hand.L");

		// タイマーを更新
		timer_.Update();
	}
	
	// チャージ中アニメーションの場合
	if (enemy_->skiningModels_["Enemy"]->animationManager_.GetPlayingAnimationName() == "Shot") {
		// アニメーションが終了しているなら
		if (!enemy_->skiningModels_["Enemy"]->animationManager_.GetIsPlayingAnimation()) {
			// 行動を変更
			enemy_->ChangeState(std::make_unique<EnemyRoot>());
			// それ以降の処理を無視
			return;
		}
	}
}

void EnemyShot::DisplayImGui()
{
}
