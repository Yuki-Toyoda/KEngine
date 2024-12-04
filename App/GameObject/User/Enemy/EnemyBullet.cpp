#include "EnemyBullet.h"
#include "App/GameObject/User/Player/Player.h"
#include "Enemy.h"
#include "Engine/GameObject/GameObjectManager.h"

void EnemyBullet::Init()
{
	// 落ち影用トランスフォームの初期化
	shadowTransform_.Init();
	shadowTransform_.SetParent(&transform_, 0b011);

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

	// 落ち影用モデル
	AddNormalModel("Shadow", &shadowTransform_, "./Resources/DropShadow", "DropShadow.obj");
	normalModels_["Shadow"]->materials_[1].color_ = Vector4(0.0f, 0.5f, 3.0f, 0.25f);

	// 弾パーティクル再生
	bulletParticle_ = ParticleManager::GetInstance()->CreateNewParticle("Bullet", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
	bulletParticle_->model_->materials_[1].tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png");
	bulletParticle_->model_->materials_[1].enableLighting_ = false;
	bulletParticle_->transform_.SetParent(&transform_);
	bulletParticle_->emitterDataBuffer_->data_->count = 1;
	bulletParticle_->emitterDataBuffer_->data_->frequency = 0.0f;
	bulletParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;

	// 軌跡パーティクルの再生
	PlayTrailParticle();

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

	// ヒットストップ中であれば更新関数呼び出し
	if (isHitStop_) {
		HitStopUpdate();
	}

	// 弾を移動させる
	transform_.translate_ = transform_.translate_ + (velocity_ * timeScale_);

	// 落ち影更新
	ShadowUpdate();

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
	// プレイヤーの剣と衝突している、かつプレイヤーに向かって弾が飛んでいるとき
	if (collider->GetColliderName() == "Sword" && isPlayer_) {
		// プレイヤーの取得
		Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");

		// プレイヤーが攻撃中の場合
		if (p->GetIsAttacking()) {
			// 敵の取得
			Enemy* e = GameObjectManager::GetInstance()->GetGameObject<Enemy>("Enemy");

			// ベクトルを敵の方向へ変更
			SetVelocity(false, e->GetRallyCount());
			isReturn_ = true;

			// 命中パーティクル再生
			PlayHitParticle();

			// ヒットストップ開始
			StartHitStop(p->GetComboManager()->GetHitStopTime() * hitStopTimeAcceleration_);
			p->StartHitStop(p->GetComboManager()->GetHitStopTime() * hitStopTimeAcceleration_);

			// 素振りの効果音の再生
			Audio::GetInstance()->PlayWave(counterSound_);
		}
		else { // 攻撃中でない場合
			// 命中パーティクル再生
			PlayHitParticle();

			// ダメージ処理を行う
			p->HitDamage(transform_.translate_);
		}
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

void EnemyBullet::SetVelocity(const bool isPlayer, const int32_t rallyCount)
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

void EnemyBullet::ShadowUpdate()
{
	// 敵の高さを元に影の大きさを調整
	float shadowScale = KLib::Lerp<float>(maxShadowScale, minShadowScale, transform_.translate_.y, maxHeight_);
	shadowTransform_.scale_ = transform_.scale_ + Vector3(shadowScale, 1.0f, shadowScale);

	// 高さは自動調整
	shadowTransform_.translate_.y = -(transform_.GetWorldPos().y) + 0.05f;
}

void EnemyBullet::DeleteBullet()
{
	// パーティクルの停止
	if (sparkParticle_ != nullptr) {
		sparkParticle_->transform_.SetParent(nullptr);
		sparkParticle_->SetIsEnd(true);
	}
	if (bulletParticle_ != nullptr) {
		bulletParticle_->transform_.SetParent(nullptr);
		bulletParticle_->SetIsEnd(true);
	}
	if (trailParticle_ != nullptr) {
		trailParticle_->transform_.SetParent(nullptr);
		trailParticle_->SetIsEnd(true);
	}

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
	trailParticle_->emitterDataBuffer_->data_->frequency = 0.025f;
	trailParticle_->emitterDataBuffer_->data_->frequencyTime = 0.25f;
}

void EnemyBullet::PlayHitParticle()
{
	// 命中パーティクル再生
	Particle* hit = ParticleManager::GetInstance()->CreateNewParticle("Hit", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.5f);
	hit->model_->materials_[1].tex_ = TextureManager::Load("BulletHitEffect.png");
	hit->model_->materials_[1].enableLighting_ = false;
	hit->transform_ = transform_.translate_;
	hit->emitterDataBuffer_->data_->count = 1;
	hit->emitterDataBuffer_->data_->frequency = 1.0f;
	hit->emitterDataBuffer_->data_->frequencyTime = 3.0f;

	// 命中破片パーティクル再生
	Particle* hitDebris = ParticleManager::GetInstance()->CreateNewParticle("HitDebris", "./Engine/Resource/Samples/Plane", "Plane.obj", 1.0f);
	hitDebris->model_->materials_[1].tex_ = TextureManager::Load("BulletHitDebrisEffect.png");
	hitDebris->model_->materials_[1].enableLighting_ = false;
	hitDebris->transform_ = transform_.translate_;
	hitDebris->emitterDataBuffer_->data_->count = 10;
	hitDebris->emitterDataBuffer_->data_->frequency = 3.0f;
	hitDebris->emitterDataBuffer_->data_->frequencyTime = 5.0f;
}

void EnemyBullet::StartHitStop(const float hitStopTime, const float timeScale)
{
	// ヒットストップ秒数が0以下の場合早期リターン
	if (hitStopTime <= 0.0f) { return; }

	// ヒットストップタイマー開始
	hitStopTimer_.Start(hitStopTime);
	// 弾パーティクルの再生を停止
	bulletParticle_->SetIsUpdate(false);

	// 速度取得
	timeScale_ = timeScale;

	// ヒットストップ中状態に
	isHitStop_ = true;
}

void EnemyBullet::HitStopUpdate()
{
	// ヒットストップタイマー終了時
	if (hitStopTimer_.GetIsFinish()) {
		// 弾パーティクルの再生を再開
		bulletParticle_->SetIsUpdate(true);
		// ヒットストップ状態終了
		isHitStop_ = false;

		// 速度リセット
		timeScale_ = 1.0f;
	}

	// ヒットストップタイマーの更新
	hitStopTimer_.Update();
}
