#include "SamplePlayer.h"

void SamplePlayer::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back( OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "Box.obj"));

	// 入力状態取得
	input_ = Input::GetInstance();

	// 色初期設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), { 1.0f, 1.0f, 1.0f });
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void SamplePlayer::Update()
{

	// 基底クラス更新
	BaseObject::Update();

	// Wキーが押されたとき
	if (input_->PushKey(DIK_W))
		transform_.translate_.z += 0.35f;
	// Sキーが押されたとき
	if (input_->PushKey(DIK_S))
		transform_.translate_.z -= 0.35f;
	// Aキーが押されたとき
	if (input_->PushKey(DIK_D))
		transform_.translate_.x += 0.35f;
	// Dキーが押されたとき
	if (input_->PushKey(DIK_A))
		transform_.translate_.x -= 0.35f;

	// スペースキーが押されたとき
	if (input_->PushKey(DIK_SPACE))
		transform_.translate_.y += 0.15f;
	// 左コントロールが押されたとき
	if (input_->PushKey(DIK_LCONTROL))
		transform_.translate_.y -= 0.15f;

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), {1.0f, 1.0f, 1.0f});
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}
}

void SamplePlayer::Draw()
{
	// オブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void SamplePlayer::AddGlobalVariables()
{
	
}

void SamplePlayer::ApplyGlobalVariables()
{
	
}

void SamplePlayer::OnCollision(BaseObject* object)
{
	switch (object->GetObjectTag())
	{
	case Camera:
		break;
	case Player:
		break;
	case Enemy:
		break;
	case Other:
		break;
	default:
		break;
	}
}
