#include "SampleFloor.h"

void SampleFloor::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	AddOBJ(&transform_, color_, "./Engine/Resource/Samples/Stage", "Stage.obj");

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), transform_.scale_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void SampleFloor::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), transform_.scale_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}
}

void SampleFloor::Draw()
{
	DrawAllOBJ();
}

void SampleFloor::AddGlobalVariables()
{
}

void SampleFloor::ApplyGlobalVariables()
{
}

void SampleFloor::OnCollisionEnter(BaseObject* object)
{
	object;
}

void SampleFloor::OnCollision(BaseObject* object)
{
	object;
}

void SampleFloor::OnCollisionExit(BaseObject* object)
{
	object;
}