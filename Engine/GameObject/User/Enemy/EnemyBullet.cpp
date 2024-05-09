#include "EnemyBullet.h"
#include "../Player/Player.h"
#include "Enemy.h"
#include "../../GameObjectManager.h"
#include "../../../Particle/ParticleEmitterManager.h"

void EnemyBullet::Init()
{
	// メッシュを追加
	AddMesh(&transform_, color_, "./Resources/MagicSphere", "MagicSphere.gltf");

	// アニメーション再生
	transform_.animations_[0].isPlay = true;

	// 球のコライダー追加
	AddColliderSphere("Bullet", &transform_.translate_, &transform_.scale_.x);

	// 効果音読み込み
	counterSound_ = Audio::GetInstance()->LoadWave("./Resources/Audio/SE/Counter.mp3");
}

void EnemyBullet::Update()
{
	// 0番目のアニメーションが終了している場合、次のアニメーションを再生する
	if (transform_.animations_[0].animationTime >= transform_.animations_[0].duration && !transform_.animations_[2].isPlay) {
		transform_.animations_[1].isPlay = true;
		transform_.animations_[1].isLoop = true;
		transform_.animations_[0].isPlay = false;
	}

	// ヒット時アニメーション再生終了時
	if (transform_.animations_[2].animationTime >= transform_.animations_[2].duration && transform_.animations_[2].isPlay) {
		transform_.animations_[1].isPlay = true;
		transform_.animations_[2].animationTime = 0.0f;
		transform_.animations_[2].isPlay = false;
	}

	// 弾を移動させる
	transform_.translate_ = transform_.translate_ + velocity_;

	// y座標が一定以下になったら削除
	if (transform_.translate_.y <= -1.0f) {
		Destroy();
	}

	// y座標が一定以上になったら削除
	if (transform_.translate_.y >= 10.0f) {
		Destroy();
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

		transform_.animations_[1].isPlay = false;
		transform_.animations_[2].animationTime = 0.0f;
		transform_.animations_[2].isPlay = true;

		/*ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 10, 5, transform_.translate_, 0.1f, 0.15f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png")); */
	}

	// ボスと衝突したら
	if (collider->GetColliderName() == "Boss" && isReturn_) {
		Enemy* e = GameObjectManager::GetInstance()->GetGameObject<Enemy>("Enemy");
		SetVelocity(true, e->GetRallyCount());
		isReturn_ = false;

		transform_.animations_[1].isPlay = false;
		transform_.animations_[2].animationTime = 0.0f;
		transform_.animations_[2].isPlay = true;

		/*ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 10, 5, transform_.translate_, 0.1f, 0.15f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png")); */
	}

	// プレイヤー、または床と衝突したら
	if (collider->GetColliderName() == "PlayerCollider"
		|| collider->GetGameObject()->GetObjectTag() == TagFloor) {
		// プレイヤーの場合ダメージ処理を行う
		if (collider->GetColliderName() == "PlayerCollider") {
			//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 10, 10, transform_.translate_, 0.1f, 0.15f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));
			// プレイヤーを取得
			Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
			// ダメージ処理を行う
			p->HitDamage(transform_.translate_);
		}

		// このオブジェクトを破壊する
		Destroy();
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
