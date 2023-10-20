#include "GameScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	Player* player = new Player();
	player->Initialize("player", BaseObject::tagPlayer);
	gameObjectManager_->AddGameObject(player); // ゲームオブジェクトマネージャーに追加

	Camera* camera = new Camera(); // インスタンス生成
	camera->Initialize("camera", BaseObject::tagCamera); // 初期化
	camera->UseThisCamera(); // 使用中のカメラに設定
	camera->transform_.translate_ = { -100.0f,0.0f,-75.0f };
	gameObjectManager_->AddGameObject(camera);

	// ステージマネージャーのインスタンス取得
	stageManager_ = StageManager::GetInstance();
	stageManager_->Initialize(); // 初期化
	stageManager_->SetStage(0);
	
	// ゲームマネージャー生成
	gameManager_ = new GameManagerObject(); // インスタンス生成
	gameManager_->SetCamera(camera); // カメラをセット
	gameManager_->SetGearTransform(&player->GetGearTransform()); // ギアのワールドトランスフォーム取得
	gameManager_->Initialize("gameManager", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(gameManager_); // マネージャーに追加

}

void GameScene::Update() {
	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
	// ステージマネージャー更新
	stageManager_->Update();
}