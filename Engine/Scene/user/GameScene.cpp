#include "GameScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

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
	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
}