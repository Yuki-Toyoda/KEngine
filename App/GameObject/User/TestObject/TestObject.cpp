#include "TestObject.h"
#include "Engine/Utility/Animation/AnimationManager.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddNormalModel("TestMesh", & transform_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");
	//AddSkiningModel(&transform_, "./Engine/Resource/Samples/Player", "Player.gltf");

	// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// OBB生成
	AddColliderAABB("Test", &transform_.translate_, &transform_.scale_);

	// 線の生成
	line_ = std::make_unique<Line>();
	line_->Init("Test", Vector3(), { 0.25f, 0.25f }, 2.5f);
	//line_->isDisplayTrail_ = true;
	line_->trailMaterial_.tex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "SwordTrail.png");

	line_->SetParent(&transform_);

	// 線用のモデル生成
	AddNormalModel("LineModel", line_->GetWorldTransform(), "./Engine/Resource/Samples/Box", "Box.obj");
}

void TestObject::Update()
{
	// ボタンを押したらパーティクル生成
	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		Particle* n = ParticleManager::GetInstance()->CreateNewParticle("Test", "./Engine/Resource/Samples/Plane", "Plane.obj", 30.0f);
		n->transform_.SetParent(&transform_);
	}

	// 線の更新
	line_->Update();
}

void TestObject::DisplayImGui()
{

	// 基底クラスのImGuiを表示する
	IObject::DisplayImGui();

	sprites_["TestSprite"]->DisplayImGui();

	ImGui::DragFloat("TransitionTime", &testFloatValue_, 0.01f, 0.0f, 5.0f);

	line_->DisplayImGui();
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
