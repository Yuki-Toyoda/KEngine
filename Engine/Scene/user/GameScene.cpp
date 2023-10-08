#include "GameScene.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient();
	ambient->Initialize("ambient", BaseObject::Other);
	gameObjectManager_->AddGameObject(ambient);
	// 箱生成
	Box* box = new Box();
	box->Initialize("box", BaseObject::Other);
	gameObjectManager_->AddGameObject(box);
	// プレイヤー生成
	MyPlayer* player = new MyPlayer();
	player->Initialize("player", BaseObject::Player);
	gameObjectManager_->AddGameObject(player);
	// 三人称カメラ生成
	TPCamera* camera = new TPCamera(); // インスタンス生成
	camera->Initialize("TPCamera", BaseObject::Camera); // 初期化
	camera->SetTarget(&player->transform_); // プレイヤーを追従対象に設定
	player->SetCameraRotation(&camera->transform_.rotate_); // プレイヤーにカメラ角度を渡す
	gameObjectManager_->AddGameObject(camera); // ゲームオブジェクトマネージャーに追加

}

void GameScene::Update() {

	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new TitleScene();
		gameObjectManager_->Initialize();
	}
}