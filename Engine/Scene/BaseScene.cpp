#include "BaseScene.h"

void BaseScene::PreInitialize()
{
	// 入力検知
	input_ = Input::GetInstance();
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	
	// カメラを生成
	camera_ = gameObjectManager_->CreateInstance<Camera>("DebugCamera", BaseObject::TagCamera);
	// このカメラを使う
	camera_->UseThisCamera();
}
