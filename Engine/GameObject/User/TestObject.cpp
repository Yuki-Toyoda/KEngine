#include "TestObject.h"
#include "../../Resource/Texture/TextureManager.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	/// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// AABB生成
	AddColliderAABB("Test", &transform_.translate_, &transform_.scale_);
}

void TestObject::Update()
{
}

void TestObject::DisplayImGui()
{
	// 基底クラスのImGuiを表示する
	BaseObject::DisplayImGui();
}

void TestObject::OnCollisionEnter(BaseObject* object)
{
	object;
	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void TestObject::OnCollisionExit(BaseObject* object)
{
	object;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
}
