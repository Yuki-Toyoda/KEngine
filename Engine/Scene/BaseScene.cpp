#include "BaseScene.h"

void BaseScene::PreInitialize()
{
	// 入力検知
	input_ = Input::GetInstance();
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// モデルデータマネージャーのインスタンス取得
	modelDataManager_ = ModelDataManager::GetInstance();

	// カメラを生成
	camera_ = gameObjectManager_->CreateInstance<Camera>("DebugCamera", IObject::TagCamera);
	// このカメラを使う
	camera_->UseThisCamera();

	// サンプルモデルの読み込み
	SampleModelLoad();
}

void BaseScene::SampleModelLoad()
{
	// サンプルモデルの読み込みをここで行う
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/Box", "Box.obj");
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/SkyDome", "SkyDome.obj");
}
