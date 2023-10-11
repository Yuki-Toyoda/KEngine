#include "Enemy.h"

void MyEnemy::Initialize(std::string name, Tag tag)
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
	objects_.push_back(OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "MonsterBody.obj"));
	objects_.push_back(OBJ::Create(&TireTransform_R_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "MonsterTire.obj"));
	objects_.push_back(OBJ::Create(&TireTransform_L_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "MonsterTire.obj"));

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

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();

}

void MyEnemy::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 接地していないなら
	if (!isLanding_) {

		if (transform_.GetParent() != nullptr) {
			transform_.translate_ = transform_.GetWorldPos();
			transform_.SetParent(nullptr);
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

		// 移動
		transform_.translate_ = transform_.translate_ + velocity_;

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

	isLanding_ = false;

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}
}

void MyEnemy::Draw()
{
	// オブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void MyEnemy::AddGlobalVariables()
{
}

void MyEnemy::ApplyGlobalVariables()
{
}

void MyEnemy::OnCollisionEnter(BaseObject* object)
{
	if (object->GetObjectTag() == Floor || object->GetObjectTag() == MoveFloor) {
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		// 接地判定On
		isLanding_ = true;
	}
}

void MyEnemy::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case Player: // 衝突したオブジェクトがプレイヤーであった場合

		// プレイヤーの座標をリセット
		object->transform_.translate_ = { 0.0f, 5.0f, 0.0f };
		object->transform_.rotate_ = { 0.0f, 0.0f, 0.0f };

		break;
	case Floor: // 衝突したオブジェクトが床の場合

		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		// 接地判定On
		isLanding_ = true;

		Vector3 nextTransform = transform_.translate_ + velocity_;
		if (nextTransform.x >= object->transform_.translate_.x + object->transform_.scale_.x ||
			nextTransform.x <= object->transform_.translate_.x - object->transform_.scale_.x) {
			if (velocity_.x < 0)
				velocity_.x = 0.15f;
			else
				velocity_.x = -0.15f;
		}

	case MoveFloor: // 衝突したオブジェクトが動く床の場合

		// とりあえず床の高さに補正
		// 接地判定On
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y;
		isLanding_ = true;
		if (transform_.GetParent() == nullptr) {

			Vector3 myWorldPos = transform_.GetWorldPos();
			Vector3 objectWorldPos = object->transform_.GetWorldPos();

			Vector3 position = myWorldPos - objectWorldPos;

			// 親の角度から回転行列を計算
			Matrix4x4 rotateMatrix = Math::MakeRotateXYZMatrix({ -object->transform_.rotate_.x , -object->transform_.rotate_.y,-object->transform_.rotate_.z });
			position = Math::TransformNormal(position, rotateMatrix);

			transform_.translate_ = position;
			transform_.SetParent(&object->transform_, 0b011);
		}

	}
}

void MyEnemy::OnCollisionExit(BaseObject* object)
{
	// 動く床から離れたなら
	if (object->GetObjectTag() == MoveFloor) {
		if (transform_.GetParent() != nullptr) {
			transform_.translate_ = transform_.GetWorldPos();
			transform_.SetParent(nullptr);
		}
	}
}
