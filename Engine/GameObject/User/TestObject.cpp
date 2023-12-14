#include "TestObject.h"
#include "../../Resource/Texture/TextureManager.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	/// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	/// 当たり判定の設定
	AABB* aabb = new AABB();
	aabb->Init(transform_.GetWorldPos(), transform_.scale_);
	collider_->SetColliderShape(aabb);
	collider_->SetGameObject(this);
}

void TestObject::Update()
{
}

void TestObject::DisplayImGui()
{
	// 基底クラスのImGuiを表示する
	BaseObject::DisplayImGui();
}
