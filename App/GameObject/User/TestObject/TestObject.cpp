#include "TestObject.h"
#include "Engine/Utility/Animation/AnimationManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Resource/Texture/TextureConverter.h"

void TestObject::Init()
{
	// テクスチャコンバーター起動
	TextureConverter converter{};
	converter.ConvertTextureWICToDDS("./Resources/uvChecker.png");

	// メッシュ追加関数
	AddNormalModel("TestMesh", & transform_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");
	normalModels_["TestMesh"]->materials_[0].tex_ = TextureManager::Load("uvChecker.dds");

	// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// OBB生成
	AddColliderAABB("Test", &transform_.translate_, &transform_.scale_);
	
	GlobalVariables* gv = GlobalVariables::GetInstance();
	gv->AddItem("Test", "TestString", "RGBFormat");
}

void TestObject::Update()
{
	// ボタンを押したらパーティクル生成
	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		Particle* n = ParticleManager::GetInstance()->CreateNewParticle("Test", "./Engine/Resource/Samples/Plane", "Plane.obj", 30.0f);
		n->transform_.SetParent(&transform_);
	}
}

void TestObject::DisplayImGui()
{

	// 基底クラスのImGuiを表示する
	IObject::DisplayImGui();

	sprites_["TestSprite"]->DisplayImGui();

	GlobalVariables* gv = GlobalVariables::GetInstance();
	std::string m = gv->GetStringValue("Test", "TestString");
	ImGui::Text(m.c_str());

	ImGui::DragFloat("TransitionTime", &testFloatValue_, 0.01f, 0.0f, 5.0f);
}

void TestObject::OnCollisionEnter(Collider* collider)
{
	collider;
}

void TestObject::OnCollisionExit(Collider* collider)
{
	collider;
}
