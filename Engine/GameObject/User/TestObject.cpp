#include "TestObject.h"
#include "../../Resource/Texture/TextureManager.h"
#include "../../Particle/ParticleEmitterManager.h"
#include "../../Utility/Animation/AnimationManager.h"

void TestObject::Init()
{
	// メッシュ追加関数
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	/// ~スプライトの追加関数~
	AddSprite("TestSprite", { 0.0f, 0.0f }, { 512.0f, 512.0f }, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));

	// OBB生成
	AddColliderOBB("Test", &transform_.scale_, &transform_.rotate_, &transform_.translate_);

	// アニメーション作成
	/*AnimationManager::GetInstance()->CreateAnimationParameter("Test");
	AnimationManager::GetInstance()->AddSelectAnimationKeys<float>("Test", "Float", &testFloatValue_);
	AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector2>("Test", "Vector2", &testVector2Value_);
	AnimationManager::GetInstance()->AddSelectAnimationKeys<Vector3>("Test", "Vector3", &testVector3Value_);*/

	line_ = std::make_unique<Line>();
	line_->Init("TestLine", transform_.translate_, {0.35f, 0.35f}, 3.0f, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
	line_->AddCollider("Line", this);
}

void TestObject::Update()
{

	line_->Update();
}

void TestObject::DisplayImGui()
{
	line_->DisplayImGui();

	// 基底クラスのImGuiを表示する
	BaseObject::DisplayImGui();
}

void TestObject::OnCollisionEnter(Collider* collider)
{
	collider;
	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };

	//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, transform_.translate_, 15.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));
}

void TestObject::OnCollisionExit(Collider* collider)
{
	collider;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, transform_.translate_, 15.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Box", "uvChecker.png"));
}
