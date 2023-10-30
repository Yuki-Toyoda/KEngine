#include "GameScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	// フェードイン
	SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f },false);
}

void GameScene::Update() {
#ifdef _DEBUG
	// デバッグ遷移
	if (Input::GetInstance()->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}