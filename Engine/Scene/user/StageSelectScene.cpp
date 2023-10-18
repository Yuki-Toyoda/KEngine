#include "StageSelectScene.h"

void StageSelectScene::Initialize()
{
	// カメラ生成
	Camera* camera = new Camera(); // インスタンス生成
	camera->Initialize("camera", BaseObject::tagCamera); // 初期化
	camera->UseThisCamera(); // 使用カメラに設定
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加
}

void StageSelectScene::Update()
{

}
