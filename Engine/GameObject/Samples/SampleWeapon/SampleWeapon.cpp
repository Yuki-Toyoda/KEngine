#include "SampleWeapon.h"

void SampleWeapon::Initialize()
{
	// 当たり判定座標
	colliderTransform_.Initialize(); // 初期化
	colliderTransform_.scale_ = { 0.5f, 0.5f, 0.5f }; // 座標設定

	// obj読み込み
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Player/Weapon", "Weapon.obj");
	// 攻撃状態false
	isAttack_ = false;

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(colliderTransform_.GetWorldPos(), colliderTransform_.scale_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);
}

void SampleWeapon::Update()
{
	// コライダー座標のオフセットを設定
	Vector3 colliderOffset = { 0.0f, 0.25f, 2.5f };
	// カメラの角度から回転行列を生成
	Matrix4x4 rotateMat = Math::MakeRotateYMatrix(target_->rotate_.y);

	// オフセットをカメラの回転に合わせて回転させる
	colliderOffset = Math::Transform(colliderOffset, rotateMat);
	colliderTransform_.translate_ = transform_.GetWorldPos() + colliderOffset;

	// 攻撃状態だったら
	if (isAttack_) {
		if (!isDestroy_) {
			// 当たり判定更新
			collider_->Update(colliderTransform_.GetWorldPos(), colliderTransform_.scale_);
			// リストに自身を登録
			collisionManager_->RegisterCollider(collider_.get());
		}
	}
}

void SampleWeapon::DisplayImGui()
{
	// ワールド座標の表示
	transform_.DisplayImGui();
}
