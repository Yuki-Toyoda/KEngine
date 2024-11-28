#include "EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "Enemy.h"
#include "Engine/GameObject/GameObjectManager.h"

void EnemyBullet::Init()
{
	// メッシュを追加
	AddNormalModel("EnemyBullet", &transform_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 拡縮を0に
	transform_.scale_ = Vector3(0.0f, 0.0f, 0.0f);

	// 弾の色調整
	normalModels_["EnemyBullet"]->materials_[0].color_ = { 0.0f, 0.5f, 3.0f, 1.0f };
	normalModels_["EnemyBullet"]->materials_[0].enableLighting_ = false;
	normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeColor_ = { 1.0f, 1.0f, 0.45f };
	normalModels_["EnemyBullet"]->materials_[0].dissolveEdgeThreshold_ = 0.1f;
	normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_ = 1.0f;

	// 球のコライダー追加
	AddColliderSphere("Bullet", &transform_.translate_, &transform_.scale_.x);

	// 効果音読み込み
	counterSound_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/Counter.mp3");
}

void EnemyBullet::Update()
{
	// 線形補間でサイズを大きく
	transform_.scale_ = KLib::Lerp<Vector3>(transform_.scale_, { 1.0f, 1.0f, 1.0f }, 0.05f);

	if (!isDissolving_) {
		normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_ = KLib::Lerp<float>(normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_, 0.0f, 0.05f);
		if (normalModels_["EnemyBullet"]->materials_[0].dissolveStrength_ <= 0.05f) {
			isDissolving_ = true;

			// 弾パーティクル再生
			bulletParticle_ = ParticleManager::GetInstance()->CreateNewParticle("Bullet", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
			bulletParticle_->model_->materials_[1].tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png");
			bulletParticle_->model_->materials_[1].enableLighting_ = false;
			bulletParticle_->transform_.SetParent(&transform_);
			bulletParticle_->emitterDataBuffer_->data_->count = 1;
			bulletParticle_->emitterDataBuffer_->data_->frequency = 0.0f;
			bulletParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;

			// 雷パーティクル再生
			sparkParticle_ = ParticleManager::GetInstance()->CreateNewParticle("BulletSpark", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
			sparkParticle_->model_->materials_[1].tex_ = TextureManager::Load("BulletSparkParticle.png");
			sparkParticle_->model_->materials_[1].enableLighting_ = false;
			sparkParticle_->transform_.SetParent(&transform_);
			sparkParticle_->emitterDataBuffer_->data_->count = 3;
			sparkParticle_->emitterDataBuffer_->data_->frequency = 0.0f;
			sparkParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;
		}
	}
	else {
		normalModels_["EnemyBullet"]->materials_[0].color_.w = KLib::Lerp<float>(normalModels_["EnemyBullet"]->materials_[0].color_.w, 0.0f, 0.15f);

		if (normalModels_["EnemyBullet"]->materials_[0].color_.w <= 0.05f) {
			normalModels_["EnemyBullet"]->isActive_ = false;
		}
	}

	// 弾を移動させる
	transform_.translate_ = transform_.translate_ + velocity_;

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

		// 命中パーティクル再生
		PlayHitParticle();

		// 素振りの効果音の再生
		Audio::GetInstance()->PlayWave(counterSound_);
	}

	// ボスと衝突したら
	if (collider->GetColliderName() == "Boss" && isReturn_) {
		Enemy* e = GameObjectManager::GetInstance()->GetGameObject<Enemy>("Enemy");
		SetVelocity(true, e->GetRallyCount());
		isReturn_ = false;

		// 命中パーティクル再生
		PlayHitParticle();
	}

	// プレイヤー、または床と衝突したら
	if (collider->GetColliderName() == "PlayerCollider"
		|| collider->GetGameObject()->GetObjectTag() == TagFloor) {
		// プレイヤーの場合ダメージ処理を行う
		if (collider->GetColliderName() == "PlayerCollider") {
			// 命中パーティクル再生
			PlayHitParticle();
			
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
	// パーティクルの停止
	sparkParticle_->SetIsEnd(true);
	bulletParticle_->SetIsEnd(true);
	trailParticle_->SetIsEnd(true);
	// 削除
	Destroy();
}

void EnemyBullet::PlayTrailParticle()
{
	// 軌跡パーティクル再生済みの場合早期リターン
	if (trailParticle_ != nullptr) { return; }

	// 軌跡パーティクル再生
	trailParticle_ = ParticleManager::GetInstance()->CreateNewParticle("BulletTrail", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
	trailParticle_->model_->materials_[1].tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png");
	trailParticle_->model_->materials_[1].enableLighting_ = false;
	trailParticle_->transform_.SetParent(&transform_);
	trailParticle_->emitterDataBuffer_->data_->count = 1;
	trailParticle_->emitterDataBuffer_->data_->frequency = 0.0f;
	trailParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;
}

void EnemyBullet::PlayHitParticle()
{
	// 命中パーティクル再生
	Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("Hit", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.5f);
	hit->model_->materials_[1].tex_ = TextureManager::Load("BulletHitEffect.png");
	hit->model_->materials_[1].enableLighting_ = false;
	hit->transform_.SetParent(&transform_);
	hit->emitterDataBuffer_->data_->count = 1;
	hit->emitterDataBuffer_->data_->frequency = 1.0f;
	hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;

	// 命中破片パーティクル再生
	Particle* hitDebris = ParticleManager::GetInstance()->CreateNewParticle("HitDebris", "./Engine/Resource/Samples/Plane", "Plane.obj", 1.0f);
	hitDebris->model_->materials_[1].tex_ = TextureManager::Load("BulletHitDebrisEffect.png");
	hitDebris->model_->materials_[1].enableLighting_ = false;
	hitDebris->transform_.SetParent(&transform_);
	hitDebris->emitterDataBuffer_->data_->count = 10;
	hitDebris->emitterDataBuffer_->data_->frequency = 3.0f;
	hitDebris->emitterDataBuffer_->data_->frequencyTime = 5.0f;
}
