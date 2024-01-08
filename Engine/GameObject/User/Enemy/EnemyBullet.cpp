#include "EnemyBullet.h"
#include "../Player/Player.h"
#include "../../GameObjectManager.h"

void EnemyBullet::Init()
{
	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 球のコライダー追加
	AddColliderSphere("Bullet", &transform_.translate_, &transform_.scale_.x);
}

void EnemyBullet::Update()
{
	// 弾を移動させる
	transform_.translate_ = transform_.translate_ + velocity_;
}

void EnemyBullet::DisplayImGui()
{

}

void EnemyBullet::OnCollisionEnter(Collider* collider)
{
	// プレイヤーの剣と衝突していたら
	if (collider->GetColliderName() == "Sword" && isPlayer_) {
		SetVelocity(false);
		isReturn_ = true;
	}

	// ボスと衝突したら
	if (collider->GetColliderName() == "Boss" && isReturn_) {
		SetVelocity(true);
		isReturn_ = false;
	}

	// プレイヤー、または床と衝突したら
	if (collider->GetColliderName() == "PlayerCollider"
		|| collider->GetGameObject()->GetObjectTag() == TagFloor) {
		// プレイヤーの場合ダメージ処理を行う
		if (collider->GetColliderName() == "PlayerCollider") {
			// プレイヤーを取得
			Player* p = GameObjectManager::GetInstance()->GetGameObject<Player>("Player");
			// ダメージ処理を行う
			p->Damage();
		}

		// このオブジェクトを破壊する
		Destroy();
	}
}

void EnemyBullet::SetVelocity(const bool& isPlayer)
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
	sub = Math::Normalize(sub);
	// 正規化したベクトルに速度を掛けて発射ベクトルを求める
	velocity_ = sub * speed_;
}
