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
	// 追従カメラ初期化
	TPCamera* tpCamera = new TPCamera(); // インスタンス生成
	tpCamera->Initialize("TPCamera", BaseObject::Camera); // 初期化
	tpCamera->UseThisCamera(); // プレイヤーを追従対象に設定
	tpCamera->SetTarget(&player ->transform_); // このカメラを使用対象に
	gameObjectManager_->AddGameObject(tpCamera); // ゲームオブジェクトマネージャーに追加
	player->SetTPCamera(tpCamera); // プレイヤーにカメラ角度を渡す

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

	// 敵
	MyEnemy* enemy = new MyEnemy(); // インスタンス生成
	enemy->Initialize("enemy", BaseObject::Enemy); // 初期化
	enemy->transform_.translate_ = { 0.0f, 5.0f, 100.0f }; // 座標設定
	gameObjectManager_->AddGameObject(enemy); // リストに追加

	// ゴールのインスタンス生成
	goal_ = std::make_unique<Goal>();
	goal_->Initialize("goal", BaseObject::StageGoal);
	goal_->transform_.translate_ = { 100.0f, 1.5f, 100.0f };
	gameObjectManager_->AddGameObject(goal_.get()); // リストに追加
}

void GameScene::Update() {

	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new TitleScene();
	}

	if (goal_->GetIsGoaled()) {
		nextScene_ = new GameScene();
	}
}