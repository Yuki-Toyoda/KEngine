#include "TestObject.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../../../Utility/Animation/AnimationManager.h"
#include "../../../Particle/ParticleManager.h"
#include "../../../Input/Input.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddNormalModel(&transform_, "./Engine/Resource/Samples/Leeme", "leeme.gltf");

	// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// OBB生成
	AddColliderAABB("Test", &transform_.translate_, &transform_.scale_);
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

	sprites_[0]->DisplayImGui();

	// ボタンを押したらパーティクル生成
	if (ImGui::Button("GenrateParticle")) {
		Particle* n = ParticleManager::GetInstance()->CreateNewParticle("Test", "./Engine/Resource/Samples/Plane", "Plane.obj", 30.0f);
		n->transform_.SetParent(&transform_);
	}
}

void TestObject::OnCollisionEnter(Collider* collider)
{
	collider;
	//meshes_[0]->material_->color_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, transform_.translate_, 15.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));
}

void TestObject::OnCollisionExit(Collider* collider)
{
	collider;
	//meshes_[0]->material_->color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	
	//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, transform_.translate_, 15.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
}
