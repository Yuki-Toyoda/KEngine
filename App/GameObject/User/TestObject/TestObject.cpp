#include "TestObject.h"
#include "Engine/Utility/Animation/AnimationManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddNormalModel("TestMesh", & transform_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

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
	/*if (Input::GetInstance()->TriggerKey(DIK_E)) {
		Particle* n = ParticleManager::GetInstance()->CreateNewParticle("Test", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
		n->transform_.SetParent(&transform_);
	}*/
}

void TestObject::DisplayImGui()
{

	// 基底クラスのImGuiを表示する
	IObject::DisplayImGui();

	// パーティクル生成時
	if (p == nullptr) {
		if (ImGui::Button("Generate Particle")) {
			p = ParticleManager::GetInstance()->CreateNewParticle("Test", "./Engine/Resource/Samples/Plane", "Plane.obj", 0.0f, true);
			p->transform_.SetParent(&transform_);
			p->emitterDataBuffer_->data_->count = 100;
		}
	}
	else {
		if (ImGui::Button("Delete Particle")) {
			p->SetIsEnd(true);
			p = nullptr;
		}
	}
}

void TestObject::OnCollisionEnter(Collider* collider)
{
	collider;
}

void TestObject::OnCollisionExit(Collider* collider)
{
	collider;
}
