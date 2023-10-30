#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void TitleScene::Initialize()
{
	// フェードイン
	SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f }, false);
}

void TitleScene::Update()
{
#ifdef _DEBUG
	// デバッグ遷移
	if (Input::GetInstance()->TriggerKey(DIK_RSHIFT)) {
		// フェードアウト
		SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 1.0f });
		SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}
