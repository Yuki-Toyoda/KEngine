#include "SampleScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"
#include "../../Particle/ParticleEmitterManager.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", BaseObject::TagCamera);

	// スカイドームの生成
	SkyDome* sk = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);
	sk->transform_.rotate_.y = -(float)std::numbers::pi / 2.0f;

	// 該当シーンのマネージャーを生成
	gameObjectManager_->CreateInstance<ObjectSceneManager>("ObjectSceneManager", BaseObject::TagNone);

	// テストオブジェクトの生成
	TestObject* test = gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagPlayer);
	test->transform_.translate_.x = -1.5f;
	test = gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	test->transform_.translate_.x = 1.5f;

	// 複雑なモデルの描画
	gameObjectManager_->CreateInstance<Rifle>("Rifle", BaseObject::TagNone);

	// パーティクル生成
	ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("test", 100, 5, {0.0f, 5.0f, 10.0f}, 20000.0f, 0.1f, TextureManager::Load("./Engine/Resource/Samples/Texture", "circle.png"));
}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
