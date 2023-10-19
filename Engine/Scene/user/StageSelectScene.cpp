#include "StageSelectScene.h"
#include "../SceneManager.h"

void StageSelectScene::Initialize()
{
	// カメラ生成
	Camera* camera = new Camera(); // インスタンス生成
	camera->Initialize("camera", BaseObject::tagCamera); // 初期化
	camera->transform_.translate_ = { 0.0f, 0.0f, -15.0f };
	camera->fov_ = 0.55f;
	camera->UseThisCamera(); // 使用カメラに設定
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	// ステージ選択シーンマネージャー生成
	stageSelectManager_ = new StageSelectManagerObject(); // インスタンス生成
	stageSelectManager_->Initialize("stageSelectManager", BaseObject::tagOther); // 初期化
	stageSelectManager_->SetCamera(camera);
	gameObjectManager_->AddGameObject(stageSelectManager_);

	// フェードイン
	SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f });
}

void StageSelectScene::Update()
{

}
