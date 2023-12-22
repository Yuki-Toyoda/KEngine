//#include "SampleEnemy.h"
//
//void SampleEnemy::Init()
//{
//	TireTransform_R_.Init();
//	TireTransform_R_.translate_ = { 0.0f, 1.0f, 0.0f };
//	TireTransform_R_.SetParent(&transform_);
//	TireTransform_L_.Init();
//	TireTransform_L_.translate_ = { 0.0f, 1.0f, 0.0f };
//	TireTransform_L_.rotate_ = { 0.0f, (float)std::numbers::pi, 0.0f };
//	TireTransform_L_.SetParent(&transform_);
//
//	// モデル読み込み
//	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Enemy/Body", "MonsterBody.obj");
//	AddMesh(&TireTransform_R_, color_, "./Engine/Resource/Samples/Enemy/Tire", "MonsterTire.obj");
//	AddMesh(&TireTransform_L_, color_, "./Engine/Resource/Samples/Enemy/Tire", "MonsterTire.obj");
//
//	// 移動速度設定
//	velocity_ = { 0.15f, 0.0f, 0.0f };
//
//	// 接地判定
//	isLanding_ = false;
//	// 最大落下速度
//	kMaxFallSpeed_ = -0.98f;
//	// 現在落下速度
//	fallSpeed_ = 0.0f;
//	// 落下加速度
//	kFallAcceleration_ = 0.0098f;
//
//	// 攻撃が当たってない
//	isHit_ = false;
//
//	// 当たり判定用aabb生成
//	AABB* aabb = new AABB();
//	aabb->Init(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
//	// 当たり判定設定
//	collider_->SetColliderShape(aabb);
//	collider_->SetGameObject(this);
//
//}
//
//void SampleEnemy::Update()
//{
//	// スティックの入力に応じて移動
//	Vector3 move = { 0.0f, 0.0f,1.0f };
//
//	// 移動量を正規化、スピードを加算
//	move = Math::Normalize(move) * 0.1f;
//	// カメラの角度から回転行列を生成
//	Matrix4x4 rotateMat = Math::MakeRotateYMatrix(transform_.rotate_.y);
//	// 移動ベクトルをカメラの角度に応じて回転させる
//	move = Math::Transform(move, rotateMat);
//	transform_.translate_ = transform_.translate_ + move;
//
//	transform_.rotate_.y += 0.025f;
//
//	// 接地していないなら
//	if (!isLanding_) {
//		// 最大落下速度を超過するまで
//		if (fallSpeed_ >= kMaxFallSpeed_) {
//			// 落下速度加算
//			fallSpeed_ -= kFallAcceleration_;
//		}
//		else {
//			// 超過していた場合は落下速度を最大速度に設定
//			fallSpeed_ = kMaxFallSpeed_;
//		}
//
//		// 最大落下速度を超過するまで
//		if (fallSpeed_ >= kMaxFallSpeed_) {
//			// 落下速度加算
//			fallSpeed_ -= kFallAcceleration_;
//		}
//		else {
//			// 超過していた場合は落下速度を最大速度に設定
//			fallSpeed_ = kMaxFallSpeed_;
//		}
//	}
//	else {
//		// 接地しているなら落下速度初期化
//		fallSpeed_ = 0.0f;
//	}
//
//	TireTransform_L_.rotate_.x += 0.01f;
//	TireTransform_R_.rotate_.x += 0.01f;
//
//	// 落下スピード加算
//	transform_.translate_.y += fallSpeed_;
//
//	// 落下したら破壊
//	if (transform_.translate_.y <= -100.0f) {
//		Destroy();
//	}
//
//	if (isHit_)
//		Destroy();
//
//	isLanding_ = false;
//
//	if (!isDestroy_) {
//		// 当たり判定更新
//		collider_->Update(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
//		// リストに自身を登録
//		collisionManager_->RegisterCollider(collider_.get());
//	}
//}
//
//void SampleEnemy::DisplayImGui()
//{
//	transform_.DisplayImGui();
//}
//
//void SampleEnemy::OnCollisionEnter(BaseObject* object)
//{
//	if (object->GetObjectTag() == TagFloor) {
//		// とりあえず床の高さに補正
//		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y * 2.0f;
//		// 接地判定On
//		isLanding_ = true;
//	}
//
//	if (object->GetObjectTag() == TagWeapon)
//		isHit_ = true;
//}
//
//void SampleEnemy::OnCollision(BaseObject* object)
//{
//	switch (object->GetObjectTag())
//	{
//	case TagFloor: // 衝突したオブジェクトが床の場合
//		// とりあえず床の高さに補正
//		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y * 2.0f;
//		// 接地判定On
//		isLanding_ = true;
//	}
//}
