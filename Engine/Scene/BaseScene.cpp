#include "BaseScene.h"
#include "../Sprite/SpriteManager.h"
#include "../Particle/ParticleEmitterManager.h"

void BaseScene::PreInitialize()
{
	// 入力検知
	input_ = Input::GetInstance();
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// モデルデータマネージャーのインスタンス取得
	modelDataManager_ = ModelDataManager::GetInstance();
	// モデルマネージャーのインスタンス取得
	modelManager_ = ModelManager::GetInstance();

	// カメラを生成
	camera_ = gameObjectManager_->CreateInstance<Camera>("DebugCamera", IObject::TagCamera);
	// このカメラを使う
	camera_->UseThisCamera();

	// 読み込み済みモデルクリア
	modelManager_->Clear();
	// 生成済みスプライトのクリア
	SpriteManager::GetInstance()->Init();
	// 生成済みパーティクルのクリア
	ParticleEmitterManager::GetInstance()->Init();

	// サンプルモデルの読み込み
	SampleModelLoad();
}

void BaseScene::SampleModelLoad()
{
	// サンプルモデルの読み込みをここで行う
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/Box", "Box.obj");
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/Plane", "Plane.obj");
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/SkyDome", "SkyDome.obj");
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/TestMultiMesh", "TestMultiMesh.gltf");
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/TestMultiMeshAnim", "TestMultiMeshAnim.gltf");
}
