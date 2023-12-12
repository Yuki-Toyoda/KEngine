#include "SampleFloor.h"

void SampleFloor::Init()
{
	// モデル読み込み
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Stage", "Stage.obj");

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Init(transform_.GetWorldPos(), transform_.scale_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);
}

void SampleFloor::Update()
{
	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), transform_.scale_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_.get());
	}
}

void SampleFloor::DisplayImGui()
{
	// ワールド座標の表示
	transform_.DisplayImGui();
}