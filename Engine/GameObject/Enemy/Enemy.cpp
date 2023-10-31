#include "Enemy.h"
void Enemy::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	TireTransform_R_.Initialize();
	TireTransform_R_.translate_ = { 0.0f, 1.0f, 0.0f };
	TireTransform_R_.SetParent(&transform_);
	TireTransform_L_.Initialize();
	TireTransform_L_.translate_ = { 0.0f, 1.0f, 0.0f };
	TireTransform_L_.rotate_ = { 0.0f, (float)std::numbers::pi, 0.0f };
	TireTransform_L_.SetParent(&transform_);

	// モデル読み込み
	AddOBJ(&transform_, color_, "./Resources/Enemy/Body", "MonsterBody.obj");
	AddOBJ(&TireTransform_R_, color_, "./Resources/Enemy/Tire", "MonsterTire.obj");
	AddOBJ(&TireTransform_L_, color_, "./Resources/Enemy/Tire", "MonsterTire.obj");

	// 移動速度設定
	velocity_ = { 0.15f, 0.0f, 0.0f };

	// 接地判定
	isLanding_ = false;
	// 最大落下速度
	kMaxFallSpeed_ = -0.98f;
	// 現在落下速度
	fallSpeed_ = 0.0f;
	// 落下加速度
	kFallAcceleration_ = 0.0098f;

	// 攻撃が当たってない
	isHit_ = false;

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();

}

void Enemy::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 接地していないなら
	if (!isLanding_) {
		// 最大落下速度を超過するまで
		if (fallSpeed_ >= kMaxFallSpeed_) {
			// 落下速度加算
			fallSpeed_ -= kFallAcceleration_;
		}
		else {
			// 超過していた場合は落下速度を最大速度に設定
			fallSpeed_ = kMaxFallSpeed_;
		}

		// 最大落下速度を超過するまで
		if (fallSpeed_ >= kMaxFallSpeed_) {
			// 落下速度加算
			fallSpeed_ -= kFallAcceleration_;
		}
		else {
			// 超過していた場合は落下速度を最大速度に設定
			fallSpeed_ = kMaxFallSpeed_;
		}
	}
	else {
		// 接地しているなら落下速度初期化
		fallSpeed_ = 0.0f;
	}

	TireTransform_L_.rotate_.x += 0.01f;
	TireTransform_R_.rotate_.x += 0.01f;

	// 落下スピード加算
	transform_.translate_.y += fallSpeed_;

	// 落下したら破壊
	if (transform_.translate_.y <= -100.0f) {
		Destroy();
	}

	if (isHit_)
		Destroy();

	isLanding_ = false;

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}
}

void Enemy::Draw()
{
	DrawAllOBJ();
}

void Enemy::AddGlobalVariables()
{
}

void Enemy::ApplyGlobalVariables()
{
}

void Enemy::OnCollisionEnter(BaseObject* object)
{
	if (object->GetObjectTag() == tagFloor) {
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y * 2.5f;
		// 接地判定On
		isLanding_ = true;
	}

	if (object->GetObjectTag() == tagWeapon)
		isHit_ = true;
}

void Enemy::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case tagFloor: // 衝突したオブジェクトが床の場合
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y * 2.5f;
		// 接地判定On
		isLanding_ = true;
	}
}

void Enemy::OnCollisionExit(BaseObject* object)
{
	object;
}