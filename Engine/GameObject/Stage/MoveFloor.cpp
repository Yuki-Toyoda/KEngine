#include "MoveFloor.h"

void StageMoveFloor::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "Box.obj"));
	objects_[0]->SetColor({ 0.0f, 0.15f, 0.75f, 1.0f });

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), transform_.scale_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void StageMoveFloor::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	if (!isReturn_) {
		if (t_ <= 5.0f) {
			transform_.translate_ = Math::EaseOut(t_, startPos_, endPos_, 5.0f);
			t_ += 1.0f / 60.0f;
		}
		else {
			t_ = 0.0f;
			isReturn_ = true;
		}
	}
	else {
		if (t_ <= 5.0f) {
			transform_.translate_ = Math::EaseOut(t_, endPos_, startPos_, 5.0f);
			t_ += 1.0f / 60.0f;
		}
		else {
			t_ = 0.0f;
			isReturn_ = false;
		}
	}

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), transform_.scale_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}
}

void StageMoveFloor::Draw()
{
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void StageMoveFloor::AddGlobalVariables()
{

}

void StageMoveFloor::ApplyGlobalVariables()
{

}

void StageMoveFloor::OnCollision(BaseObject* object)
{
	object;
}

void StageMoveFloor::OnCollisionExit(BaseObject* object)
{
	object;
}
