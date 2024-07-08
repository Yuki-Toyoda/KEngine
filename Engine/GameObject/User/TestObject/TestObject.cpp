#include "TestObject.h"
#include "../../../Resource/Texture/TextureManager.h"
#include "../../../Particle/ParticleEmitterManager.h"
#include "../../../Utility/Animation/AnimationManager.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddNormalModel(&transform_, "./Engine/Resource/Samples/Plane", "Plane.obj");

	// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// OBB生成
	//AddColliderOBB("Test", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	AddColliderAABB("Test", &transform_.translate_, &transform_.scale_);

	//// アニメーションのパラメータ作成
	//AnimationManager::GetInstance()->CreateAnimationParameter("Test");
	//AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test", "Scale");
	//AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test", "Rotate");
	//AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test", "Translate");

	//AnimationManager::GetInstance()->CreateAnimationParameter("Test2");
	//AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test2", "Scale");
	//AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test2", "Rotate");
	//AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test2", "Translate");

	//// アニメーションの作成
	//animation_ = AnimationManager::GetInstance()->CreateAnimation("TestAnimation", "Test");
	//animation_->AddAnimationKeys<Vector3>("Scale", &transform_.scale_);
	//animation_->AddAnimationKeys<Vector3>("Rotate", &transform_.rotate_);
	//animation_->AddAnimationKeys<Vector3>("Translate", &transform_.translate_);

	/*line_ = std::make_unique<Line>();
	line_->Init("TestLine", transform_.translate_, {0.35f, 0.35f}, 3.0f, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
	line_->AddCollider("Line", this);*/

	/*transform_.animations_[0].isLoop = true;
	transform_.animations_[0].isPlay = true;*/
}

void TestObject::Update()
{
	// 線更新
	//line_->Update();
}

void TestObject::DisplayImGui()
{

	// 基底クラスのImGuiを表示する
	//BaseObject::DisplayParameterImGui();

	// 表示状態の切り替え
	ImGui::Checkbox("isActive", &isActive_);

	transform_.DisplayImGui();

	if (ImGui::Button("Delete This")) {
		Destroy();
	}

	sprites_[0]->DisplayImGui();
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
