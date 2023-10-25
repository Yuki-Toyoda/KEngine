#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void TitleScene::Initialize()
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

	// タイトルマネージャーオブジェクト生成
	titleManager_ = new TitleManagerObject(); // インスタンス生成
	titleManager_->SetCamera(camera);
	titleManager_->Initialize("titleManagerObject", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(titleManager_); // ゲームオブジェクトマネージャーに追加

}

void TitleScene::Update()
{
	// タイトルからゲームシーンへの遷移を行うよう命令されたら遷移する
	if (titleManager_->GetIsGoStageSelectSceneScene()) {
		BaseScene* nextScene = new StageSelectScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
}
