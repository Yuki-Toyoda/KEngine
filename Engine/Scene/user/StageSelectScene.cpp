#include "StageSelectScene.h"
#include "../SceneManager.h"

void StageSelectScene::Initialize()
{
	// カメラ生成
	Camera* camera = new Camera(); // インスタンス生成
	camera->Initialize("camera", BaseObject::tagCamera); // 初期化
	camera->transform_.translate_ = { 0.0f, 0.0f, -12.5f };
	camera->fov_ = 0.4f;
	camera->UseThisCamera(); // 使用カメラに設定
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	// ステージ選択マネージャ
	stageManager_ = StageManager::GetInstance(); // インスタンス取得
	stageManager_->Initialize(); // 初期化

	// ステージ選択シーンマネージャー生成
	stageSelectManager_ = new StageSelectManagerObject(); // インスタンス生成
	stageSelectManager_->SetCamera(camera); // カメラをセット
	stageSelectManager_->Initialize("stageSelectManager", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(stageSelectManager_);

}

void StageSelectScene::Update()
{

	// ゲームシーンへ遷移させるなら
	if (stageSelectManager_->IsGoGameScene()) {
		SceneManager::GetInstance()->SetSelectedStageNumber(stageSelectManager_->GetSelectedStageNumber());
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
}
