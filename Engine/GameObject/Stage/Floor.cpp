#include "Floor.h"

void Floor::Initialize(std::string name, Tag tag)
{
	// 基底クラス初期化
	BaseObject::Initialize(name, tag);
	isActive_ = true;

	// モデル読み込み
	objects_.push_back(OBJ::Create(&transform_, { 1.0f, 1.0f, 1.0f, 1.0f }, "./Resources", "Stage.obj"));

	// 
	transform_.scale_ = { 1.0f, 1.0f, 1.0f };

	// 衝突属性を設定
	collider_->SetCollisionAttribute(0xfffffffd);
	// 衝突対象を自分の属性以外に設定
	collider_->SetCollisionMask(0x00000002);

	// 当たり判定用aabb生成
	AABB* aabb = new AABB();
	aabb->Initialize(transform_.GetWorldPos(), transform_.scale_);
	// 当たり判定設定
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);

	// グローバル変数に調整したい値を追加
	AddGlobalVariables();
}

void Floor::Update()
{
	// 基底クラス更新
	BaseObject::Update();

	if (!isDestroy_) {
		// 当たり判定更新
		collider_->Update(transform_.GetWorldPos(), transform_.scale_);
		// リストに自身を登録
		collisionManager_->RegisterCollider(collider_);
	}
}

void Floor::Draw()
{
	// カメラオブジェクトの描画
	for (OBJ* obj : objects_) {
		if (isActive_) {
			obj->Draw();
		}
	}
}

void Floor::AddGlobalVariables()
{

}

void Floor::ApplyGlobalVariables()
{

}

void Floor::OnCollisionEnter(BaseObject* object)
{

}
