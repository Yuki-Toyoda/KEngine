#include "EnemyBullet.h"

void EnemyBullet::Init()
{
	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 球のコライダー追加
	AddColliderSphere("Sphere", &transform_.translate_, &transform_.scale_.x);
}

void EnemyBullet::Update()
{

}

void EnemyBullet::DisplayImGui()
{

}

void EnemyBullet::OnCollisionEnter(Collider* collider)
{
	// プレイヤー、または床と衝突したら
	if (collider->GetColliderName() == "PlayerCollider"
		|| collider->GetGameObject()->GetObjectTag() == TagFloor) {
		// このオブジェクトを破壊する
		Destroy();
	}

	// プレイヤーの剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		// プレイヤーから敵への方向ベクトルを求める
		Vector3 sub = player_->translate_ - enemy_->translate_;
		// 求めた差分ベクトルを正規化
		sub = Math::Normalize(sub);
		// 正規化したベクトルに速度を掛けて発射ベクトルを求める
		velocity_ = sub * speed_;
	}
}
