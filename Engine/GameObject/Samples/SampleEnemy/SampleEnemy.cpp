#include "SampleEnemy.h"
#include "../../GameObjectManager.h"
#include "../SampleThirdPersonCamera/ThirdPersonCamera.h"

void SampleEnemy::Initialize()
{
	TireTransform_R_.Initialize();
	TireTransform_R_.translate_ = { 0.0f, 1.0f, 0.0f };
	TireTransform_R_.SetParent(&transform_);
	TireTransform_L_.Initialize();
	TireTransform_L_.translate_ = { 0.0f, 1.0f, 0.0f };
	TireTransform_L_.rotate_ = { 0.0f, (float)std::numbers::pi, 0.0f };
	TireTransform_L_.SetParent(&transform_);

	// モデル読み込み
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Enemy/Body", "MonsterBody.obj", true);
	AddMesh(&TireTransform_R_, color_, "./Engine/Resource/Samples/Enemy/Tire", "MonsterTire.obj", true);
	AddMesh(&TireTransform_L_, color_, "./Engine/Resource/Samples/Enemy/Tire", "MonsterTire.obj", true);

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

	// 敵のHP
	hp_ = 3;
	// クールタイムリセット
	hitCoolTime_ = 0;

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

}

void SampleEnemy::Update()
{
	// 吹っ飛び状態でないとき
	if (awaySpeed_ <= 0.0f) {
		// スティックの入力に応じて移動
		Vector3 move = { 0.0f, 0.0f,1.0f };

		// 移動量を正規化、スピードを加算
		move = Math::Normalize(move) * 0.1f;
		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Math::MakeRotateYMatrix(transform_.rotate_.y);
		// 移動ベクトルをカメラの角度に応じて回転させる
		move = Math::Transform(move, rotateMat);
		transform_.translate_ = transform_.translate_ + move;

		transform_.rotate_.y += 0.025f;
	}	

	// ダメージ処理
	if (hp_ > 0) {
		Damage();
	}
	else {
		Dead();
	}

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

	if (hitCoolTime_ > 0) {
		// クールタイムデクリメント
		hitCoolTime_--;
	}
	else {
		hitCoolTime_ = 0;
	}

	isLanding_ = false;

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}
}

void SampleEnemy::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::InputInt("HP", &hp_);
}

void SampleEnemy::OnCollisionEnter(BaseObject* object)
{
	if (object->GetObjectTag() == TagFloor) {
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y * 2.0f;
		// 接地判定On
		isLanding_ = true;
	}

	// 武器と当たったら
	if (object->GetObjectTag() == TagWeapon && hitCoolTime_ <= 0) {
		// HPを減らす
		hp_--;

		// 吹っ飛びベクトルの設定
		if (hp_ > 0) {
			// 使用中のカメラｗぽ取得
			ThirdPersonCamera* camera = GameObjectManager::GetInstance()->GetGameObject<ThirdPersonCamera>("TPCamera");
			// 敵が今向いている方向の反対ベクトルを取る
			Matrix4x4 rotateMat = Math::MakeRotateYMatrix(camera->transform_.rotate_.y);
			awayVector_ = {0.0f, 0.0f, 1.0f};
			awayVector_ = Math::Normalize(Math::Transform(awayVector_, rotateMat));
			awayVector_.y = 0.5f;
			// スピードを設定
			awaySpeed_ = 1.0f;
		}
		else {
			// 使用中のカメラｗぽ取得
			ThirdPersonCamera* camera = GameObjectManager::GetInstance()->GetGameObject<ThirdPersonCamera>("TPCamera");
			// 敵が今向いている方向の反対ベクトルを取る
			Matrix4x4 rotateMat = Math::MakeRotateYMatrix(camera->transform_.rotate_.y);
			awayVector_ = { 0.0f, 0.0f, 1.0f };
			awayVector_ = Math::Normalize(Math::Transform(awayVector_, rotateMat));
			awayVector_.y = 0.5f;
			// スピードを設定
			awaySpeed_ = 0.5f;
		}

		// 命中クールタイムリセット
		hitCoolTime_ = kHitCoolTime;
	}
}

void SampleEnemy::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case TagFloor: // 衝突したオブジェクトが床の場合
		// とりあえず床の高さに補正
		transform_.translate_.y = object->transform_.translate_.y + object->transform_.scale_.y * 2.0f;
		// 接地判定On
		isLanding_ = true;
	}
}

void SampleEnemy::Damage()
{
	// 吹っ飛びスピードが0でなければ
	if (awaySpeed_ > 0) {
		// 吹っ飛び方向を求める
		Vector3 away = awayVector_* awaySpeed_;
		// 移動させる
		transform_.translate_ = transform_.translate_ + away;

		// 吹っ飛びスピードを少しづつ遅くする
		awaySpeed_ -= 0.1f;
	}
	else {
		// 吹っ飛びスピードを0
		awaySpeed_ = 0.0f;
	}
}

void SampleEnemy::Dead()
{
	// 吹っ飛び方向を求める
	Vector3 away = awayVector_ * awaySpeed_;
	// 移動させる
	transform_.translate_ = transform_.translate_ + away;

	// 吹っ飛びスピードを少しづつ遅くする
	color_.w = color_.w -= 0.01f;

	// 吹っ飛びスピードを徐々に上げる
	awaySpeed_ += 0.001f;

	if (color_.w <= 0.0f) {
		Destroy();
	}
}
