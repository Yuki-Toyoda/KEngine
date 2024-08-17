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
	bullet_ = GameObjectManager::GetInstance()->CreateInstance<EnemyBullet>("EnemyBullet", IObject::TagEnemy);
	// 弾の座標を左手に合わせる
	bullet_->transform_.translate_ = enemy_->armTransform_L_.GetWorldPos() + enemy_->armTransform_L_.translate_;

	// プレイヤーの座標をセット
	Player* player = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
	bullet_->SetPlayerTransform(&player->colliderTransform_);
	// 敵の座標をセット
	bullet_->SetEnemyTransform(&enemy_->transform_);

	// チャージパーティクル再生
	Particle* charge = ParticleManager::GetInstance()->CreateNewParticle("Charge", "./Engine/Resource/Samples/Plane", "Plane.obj", 2.5f);
	charge->model_->materials_[1].tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png");
	charge->model_->materials_[1].enableLighting_ = false;
	charge->transform_.translate_ = bullet_->transform_.translate_;
	charge->emitterDataBuffer_->data_->count = 10;
	charge->emitterDataBuffer_->data_->frequency = 0.25f;
	charge->emitterDataBuffer_->data_->frequencyTime = 0.25f;

	// アニメーションのループ設定を切る
	enemy_->enemyAnim_->isLoop_ = false;

	// アニメーションを変更
	if (enemy_->enemyAnim_->GetReadingParameterName() != "Enemy_Charge") {
		enemy_->enemyAnim_->ChangeParameter("Enemy_Charge", 0.1f, true);
	}

	// タイマーの初期化
	timer_.Start(KLib::RandomF(kMinShotTime_, kMaxShotTime_, 2));
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
			enemy_->enemyAnim_->ChangeParameter("Enemy_Charging", 0.1f, true);
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
			enemy_->enemyAnim_->ChangeParameter("Enemy_Shot", 0.1f, true);
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
