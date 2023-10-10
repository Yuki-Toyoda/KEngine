#include "GameScene.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::Other); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加
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

	StageFloor* floor1 = new StageFloor(); // インスタンス生成
	floor1->Initialize("StageFloor", BaseObject::Floor); // 初期化
	floor1->transform_.translate_ = { 0.0f, 0.0f, 0.0f };
	floor1->transform_.scale_ = { 15.0f, 0.5f, 15.0f };
	gameObjectManager_->AddGameObject(floor1); // リストに追加

	StageFloor* floor2 = new StageFloor(); // インスタンス生成
	floor2->Initialize("StageFloor", BaseObject::Floor); // 初期化
	floor2->transform_.translate_ = { 0.0f, 0.0f, 100.0f };
	floor2->transform_.scale_ = { 15.0f, 0.5f, 15.0f };
	gameObjectManager_->AddGameObject(floor2); // リストに追加

	StageFloor* floor3 = new StageFloor(); // インスタンス生成
	floor3->Initialize("StageFloor", BaseObject::Floor); // 初期化
	floor3->transform_.translate_ = { 100.0f, 0.0f, 100.0f };
	floor3->transform_.scale_ = { 15.0f, 0.5f, 15.0f };
	gameObjectManager_->AddGameObject(floor3); // リストに追加

	// ステージの動く床生成
	StageMoveFloor* moveFloor1 = new StageMoveFloor(); // インスタンス生成
	moveFloor1->Initialize("moveFloor", BaseObject::MoveFloor); // 初期化
	moveFloor1->transform_.scale_ = { 10.0f, 0.5f, 10.0f };
	moveFloor1->SetStartPos({ 0.0f, 0.0f, 25.0f });
	moveFloor1->SetEndPos({ 0.0f, 0.0f, 75.0f });
	gameObjectManager_->AddGameObject(moveFloor1); // リストに追加

	StageMoveFloor* moveFloor2 = new StageMoveFloor(); // インスタンス生成
	moveFloor2->Initialize("moveFloor", BaseObject::MoveFloor); // 初期化
	moveFloor2->transform_.scale_ = { 10.0f, 0.5f, 10.0f };
	moveFloor2->SetStartPos({ 25.0f, 0.0f, 100.0f });
	moveFloor2->SetEndPos({ 75.0f, 0.0f, 100.0f });
	gameObjectManager_->AddGameObject(moveFloor2); // リストに追加

	// ゴールのインスタンス生成
	goal_ = std::make_unique<Goal>();
	goal_->Initialize("goal", BaseObject::StageGoal);
	goal_->transform_.translate_ = { 100.0f, 1.5f, 100.0f };
	gameObjectManager_->AddGameObject(goal_.get()); // リストに追加
}

void GameScene::Update() {

	if (goal_->GetIsGoaled()) {
		nextScene_ = new GameScene();
	}
}