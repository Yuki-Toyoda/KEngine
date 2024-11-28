#include "EnemyShot.h"
#include "App/GameObject/User/Enemy/Enemy.h"
#include "App/GameObject/User/Enemy/EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "Engine/GameObject/GameObjectManager.h"

void EnemyShot::Init()
{
	// 行動名設定
	stateName_ = "Shot";

	// 弾を生成する
	bullet_ = GameObjectManager::GetInstance()->CreateInstance<EnemyBullet>("EnemyBullet", IObject::TagEnemy);
	// 弾の座標を左手に合わせる
	bullet_->transform_.translate_ = enemy_->GetLeftArmPosition();

	// プレイヤーの座標をセット
	Player* player = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	bullet_->SetPlayerTransform(player->GetBodyPos());
	// 敵の座標をセット
	bullet_->SetEnemyTransform(&enemy_->transform_);

	// アニメーションのループ設定を切る
	anim_->isLoop_ = false;

	// アニメーションを変更
	if (anim_->GetReadingParameterName() != "Enemy_Charge") {
		anim_->ChangeParameter("Enemy_Charge", 0.1f, true);
	}

	// タイマーの初期化
	timer_.Start(KLib::RandomF(kMinShotTime_, kMaxShotTime_, 2));
}

void EnemyShot::Update()
{
	// チャージアニメーションの場合
	if (anim_->GetReadingParameterName() == "Enemy_Charge") {
		// アニメーションが終了しているなら
		if (anim_->isEnd_) {
			// アニメーションのループを行う
			anim_->isLoop_ = true;
			// 次のアニメーションを設定
			anim_->ChangeParameter("Enemy_Charging", 0.1f, true);
		}
	}

	// チャージ中アニメーションの場合
	if (anim_->GetReadingParameterName() == "Enemy_Charging") {
		// タイマー終了時
		if (timer_.GetIsFinish()) {
			// アニメーションのループを行う
			anim_->isLoop_ = false;
			// 次のアニメーションを設定
			anim_->ChangeParameter("Enemy_Shot", 0.1f, true);
			// プレイヤーに向かって弾を発射する
			bullet_->SetVelocity(true, enemy_->GetRallyCount());
			// 弾の軌跡パーティクルの再生
			bullet_->PlayTrailParticle();

			// その後の処理を強制終了
			return;
		}

		// タイマーを更新
		timer_.Update();
	}
	
	// チャージ中アニメーションの場合
	if (anim_->GetReadingParameterName() == "Enemy_Shot") {
		// アニメーションが終了しているなら
		if (anim_->isEnd_) {
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
