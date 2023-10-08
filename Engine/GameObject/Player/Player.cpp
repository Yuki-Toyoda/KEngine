#include "Player.h"

void MyPlayer::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create({ 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "Box.obj"));

	// 入力状態取得
	input_ = Input::GetInstance();
	// コントローラー入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 衝突属性を設定
	collider_->SetCollisionAttribute(0xfffffffe);
	// 衝突対象を自分の属性以外に設定
	collider_->SetCollisionMask(0x00000001);

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void MyPlayer::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	// 入力取得
	preJoyState_ = joyState_; // 前フレームの入力取得
	input_->GetJoystickState(0, joyState_); // 現在フレームの入力取得

	// 移動速度
	const float speed = 0.3f;

	// スティックの入力に応じて移動
	Vector3 move = {
		(float)joyState_.Gamepad.sThumbLX / SHRT_MAX, 0.0f,
		(float)joyState_.Gamepad.sThumbLY / SHRT_MAX };

	// 移動量を正規化、スピードを加算
	move = Math::Normalize(move) * speed;

	// カメラ回転角がセットされている場合
	if (cameraRotation_ != nullptr) {
		// カメラの角度から回転行列を生成
		Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(*cameraRotation_);
		// 移動ベクトルをカメラの角度に応じて回転させる
		move = Math::Transform(move, rotateMat);
	}

	// 移動
	transform_.translate_ = transform_.translate_ + move;

	// 移動方向にオブジェクトの向きを合わせる
	transform_.rotate_.y = atan2(move.x, move.z);

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), {1.0f, 1.0f, 1.0f});
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}

}

void MyPlayer::Draw()
{
	// オブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->SetWorldTransform(transform_);
			obj->Draw();
		}
	}
}

void MyPlayer::AddGlobalVariables()
{

}

void MyPlayer::ApplyGlobalVariables()
{

}

void MyPlayer::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case Camera:
		break;
	case Enemy:
		break;
	case Other:
		break;
	default:
		break;
	}
}
