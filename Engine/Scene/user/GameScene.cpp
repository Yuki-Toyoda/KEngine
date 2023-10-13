#include "GameScene.h"
#include "../../GameObject/SampleList.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	SampleBox* sampleBox = new SampleBox();
	sampleBox->Initialize("sampleBox", BaseObject::tagOther);
	gameObjectManager_->AddGameObject(sampleBox); // ゲームオブジェクトマネージャーに追加
}

void GameScene::Update() {
	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new TitleScene();
	}
}