#include "GameScene.h"

void GameScene::Initialize() {
	
	// 箱生成
	Box* box = new Box();
	box->Initialize("box", BaseObject::Other);
	gameObjectManager_->AddGameObject(box);
	// プレイヤー生成
	SamplePlayer* player = new SamplePlayer();
	player->Initialize("player", BaseObject::Player);
	gameObjectManager_->AddGameObject(player);

}

void GameScene::Update() {

	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new TitleScene();
		gameObjectManager_->Initialize();
	}
}