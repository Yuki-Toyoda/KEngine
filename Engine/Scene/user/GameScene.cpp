#include "GameScene.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::Other); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加
	// 箱生成
	Box* box = new Box(); // インスタンス生成
	box->Initialize("box", BaseObject::Other); // 初期化
	gameObjectManager_->AddGameObject(box); // ゲームオブジェクトマネージャーに追加
	// プレイヤー生成
	MyPlayer* player = new MyPlayer();
	player->Initialize("player", BaseObject::Player); // 初期化
	player->transform_.translate_ = { 0.0f, 5.0f, 0.0f };
	gameObjectManager_->AddGameObject(player); // ゲームオブジェクトマネージャーに追加
	// 三人称カメラ生成
	TPCamera* camera = new TPCamera(); // インスタンス生成
	camera->Initialize("TPCamera", BaseObject::Camera); // 初期化
	camera->SetUseThisCamera(true); // プレイヤーを追従対象に設定
	camera->SetTarget(&player->transform_); // このカメラを使用対象に
	player->SetCameraRotation(&camera->transform_.rotate_); // プレイヤーにカメラ角度を渡す
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

	// ステージマネージャー生成
	stageManager_ = std::make_unique<StageManager>(); // インスタンス生成
	stageManager_->Initialize(); // 初期化
}

void GameScene::Update() {

	// ステージマネージャー更新
	stageManager_->Update();

	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new TitleScene();
		gameObjectManager_->Initialize();
	}
}