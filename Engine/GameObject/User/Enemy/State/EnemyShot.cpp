#include "EnemyShot.h"
#include "../Enemy.h"
#include "../EnemyBullet.h"
#include "../../Player/Player.h"
#include "../../../GameObjectManager.h"

void EnemyShot::Init()
{
	// 行動名設定
	stateName_ = "Shot";

	// 弾を生成する
	bullet_ = GameObjectManager::GetInstance()->CreateInstance<EnemyBullet>("EnemyBullet", BaseObject::TagEnemy);
	// 弾の座標を左手に合わせる
	bullet_->transform_.translate_ = enemy_->armTransform_L_.GetWorldPos() + enemy_->armTransform_L_.translate_;
	// プレイヤーの座標をセット
	Player* player = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	bullet_->SetPlayerTransform(&player->headTransform_);
	// 敵の座標をセット
	bullet_->SetEnemyTransform(&enemy_->transform_);

	// アニメーションのループ設定を切る
	enemy_->enemyAnim_->isLoop_ = false;

	// アニメーションを変更
	if (enemy_->enemyAnim_->GetReadingParameterName() != "Enemy_Charge") {
		enemy_->enemyAnim_->ChangeParameter("Enemy_Charge", true);
	}

	// タイマーの初期化
	timer_.Start(Math::RandomF(kMinShotTime_, kMaxShotTime_, 2));
}

void EnemyShot::Update()
{
	// チャージアニメーションの場合
	if (enemy_->enemyAnim_->GetReadingParameterName() == "Enemy_Charge") {
		// アニメーションが終了しているなら
		if (enemy_->enemyAnim_->isEnd_) {
			// アニメーションのループを行う
			enemy_->enemyAnim_->isLoop_ = true;
			// 次のアニメーションを設定
			enemy_->enemyAnim_->ChangeParameter("Enemy_Charging", true);
		}

		// 弾の座標を左手に合わせる
		//bullet_->transform_.translate_ = enemy_->armTransform_L_.GetWorldPos() + enemy_->armTransform_L_.translate_;
	}

	// チャージ中アニメーションの場合
	if (enemy_->enemyAnim_->GetReadingParameterName() == "Enemy_Charging") {
		// タイマー終了時
		if (timer_.GetIsFinish()) {
			// アニメーションのループを行う
			enemy_->enemyAnim_->isLoop_ = false;
			// 次のアニメーションを設定
			enemy_->enemyAnim_->ChangeParameter("Enemy_Shot", true);
			// プレイヤーに向かって弾を発射する
			bullet_->SetVelocity(true, enemy_->GetRallyCount());
			// 

			// その後の処理を強制終了
			return;
		}

		// 弾の座標を左手に合わせる
		//bullet_->transform_.translate_ = enemy_->armTransform_L_.GetWorldPos() + enemy_->armTransform_L_.translate_;
		
		// タイマーを更新
		timer_.Update();
	}
	
	// チャージ中アニメーションの場合
	if (enemy_->enemyAnim_->GetReadingParameterName() == "Enemy_Shot") {
		// アニメーションが終了しているなら
		if (enemy_->enemyAnim_->isEnd_) {
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
