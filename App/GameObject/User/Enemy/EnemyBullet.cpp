#include "EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "Enemy.h"
#include "Engine/GameObject/GameObjectManager.h"

void EnemyBullet::Init()
{
	// メッシュを追加
	AddNormalModel("EnemyBullet", &transform_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 弾の色調整
	normalModels_["EnemyBullet"]->materials_[0].color_ = { 0.0f, 2.5f, 1.0f, .85f };
	normalModels_["EnemyBullet"]->materials_[0].enableLighting_ = false;
	normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeColor_ = { 1.0f, 1.0f, 0.45f };
	normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_ = 0.1f;
	normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_ = 1.0f;

	// 軌跡パーティクル再生
	trailParticle_ = ParticleManager::GetInstance()->CreateNewParticle("BulletTrail", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
	trailParticle_->model_->materials_[1].tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png");
	trailParticle_->model_->materials_[1].enableLighting_ = false;
	trailParticle_->transform_.translate_ = transform_.translate_;
	trailParticle_->emitterDataBuffer_->data_->count = 10;
	trailParticle_->emitterDataBuffer_->data_->frequency = 0.25f;
	trailParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;

	// 球のコライダー追加
	AddColliderSphere("Bullet", &transform_.translate_, &transform_.scale_.x);

	// 効果音読み込み
	counterSound_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/Counter.mp3");
}

void EnemyBullet::Update()
{
	// 軌跡パーティクル発生座標を調整する
	trailParticle_->transform_.translate_ = transform_.translate_;

	if (isDissolving_) {
		if (!isSwitchDissolving_) {
			normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_ = KLib::Lerp<float>(normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_, 0.35f, 0.05f);
			if (normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_ >= 0.325f) {
				isSwitchDissolving_ = true;
			}
		}
		else {
			normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_ = KLib::Lerp<float>(normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_, 0.075f, 0.05f);
			if (normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_ <= 0.1f) {
				isSwitchDissolving_ = false;
			}
		}
	}
	else {
		normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_ = KLib::Lerp<float>(normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_, 0.3f, 0.05f);
		if (normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_ <= 0.35f) {
			isDissolving_ = true;
		}
	}

	// 弾を移動させる
	transform_.translate_ = transform_.translate_ + velocity_;

	// UVを動かし続ける
	normalModels_["EnemyBullet"]->materials_[0].uvTransform_.translate_.x += 0.01f;

	// y座標が一定以下になったら削除
	if (transform_.translate_.y <= -1.0f) {
		DeleteBullet();
	}

	// y座標が一定以上になったら削除
	if (transform_.translate_.y >= 10.0f) {
		DeleteBullet();
	}
}

void EnemyBullet::DisplayImGui()
{

}

void EnemyBullet::OnCollisionEnter(Collider* collider)
{
	// プレイヤーの剣と衝突していたら
	if (collider->GetColliderName() == "Sword" && isPlayer_) {
		Enemy* e = GameObjectManager::GetInstance()->GetGameObject<Enemy>("Enemy");

		SetVelocity(false, e->GetRallyCount());
		isReturn_ = true;

		// 素振りの効果音の再生
		Audio::GetInstance()->PlayWave(counterSound_);

		// 命中パーティクル再生
		Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("Hit", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.5f);
		hit->model_->materials_[1].tex_ = TextureManager::Load("BulletHitEffect.png");
		hit->model_->materials_[1].enableLighting_ = false;
		hit->transform_.translate_ = transform_.translate_;
		hit->emitterDataBuffer_->data_->count = 1;
		hit->emitterDataBuffer_->data_->frequency = 1.0f;
		hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;
	}

	// ボスと衝突したら
	if (collider->GetColliderName() == "Boss" && isReturn_) {
		Enemy* e = GameObjectManager::GetInstance()->GetGameObject<Enemy>("Enemy");
		SetVelocity(true, e->GetRallyCount());
		isReturn_ = false;

		// 命中パーティクル再生
		Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("EnemyHit", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.5f);
		hit->model_->materials_[1].tex_ = TextureManager::Load("BulletHitEffect.png");
		hit->model_->materials_[1].enableLighting_ = false;
		hit->transform_.translate_ = transform_.translate_;
		hit->emitterDataBuffer_->data_->count = 1;
		hit->emitterDataBuffer_->data_->frequency = 1.0f;
		hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;

	}

	// プレイヤー、または床と衝突したら
	if (collider->GetColliderName() == "PlayerCollider"
		|| collider->GetGameObject()->GetObjectTag() == TagFloor) {
		// プレイヤーの場合ダメージ処理を行う
		if (collider->GetColliderName() == "PlayerCollider") {
			// 命中パーティクル再生
			Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("EnemyHit", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.5f);
			hit->model_->materials_[1].tex_ = TextureManager::Load("HitEffect.png");
			hit->model_->materials_[1].enableLighting_ = false;
			hit->transform_.translate_ = transform_.translate_;
			hit->emitterDataBuffer_->data_->count = 1;
			hit->emitterDataBuffer_->data_->frequency = 1.0f;
			hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;
			// プレイヤーを取得
			Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
			// ダメージ処理を行う
			p->HitDamage(transform_.translate_);
		}

		// このオブジェクトを破壊する
		DeleteBullet();
	}
}

void EnemyBullet::SetVelocity(const bool& isPlayer, const int32_t& rallyCount)
{
	// 差分ベクトル格納用
	Vector3 sub = Vector3(0.0f, 0.0f, 0.0f);

	if (isPlayer) {
		// プレイヤーから敵への方向ベクトルを求める
		sub = player_->GetWorldPos() - transform_.GetWorldPos();
		isPlayer_ = true;
	}
	else {
		// プレイヤーから敵への方向ベクトルを求める
		sub = enemy_->GetWorldPos() - transform_.GetWorldPos();
		isPlayer_ = false;
	}
	// 求めた差分ベクトルを正規化
	sub = Vector3::Normalize(sub);
	// 正規化したベクトルに速度を掛けて発射ベクトルを求める
	if (rallyCount <= 0) {
		velocity_ = sub * speed_;
	}
	else {
		velocity_ = sub * (speed_ + kAcceleration_ * (float)rallyCount);
	}
}

void EnemyBullet::DeleteBullet()
{
	// 軌跡用パーティクルの停止
	trailParticle_->SetIsEnd(true);
	// 削除
	Destroy();
}
