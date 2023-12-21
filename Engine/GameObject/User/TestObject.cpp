#include "TestObject.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../../Particle/ParticleEmitterManager.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	/// ~スプライトの追加関数~
	//AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// OBB生成
	AddColliderOBB("Test", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
}

void TestObject::Update()
{
}

void TestObject::DisplayImGui()
{
	// 基底クラスのImGuiを表示する
	BaseObject::DisplayImGui();
}

void TestObject::OnCollisionEnter(Collider* collider)
{
	collider;
	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, transform_.translate_, 15.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));
}

void TestObject::OnCollisionExit(Collider* collider)
{
	collider;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, transform_.translate_, 15.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
}
