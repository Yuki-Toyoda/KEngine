#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{
	SampleBox* box = nullptr;
	box = gameObjectManager_->CreateInstance<SampleBox>("box", BaseObject::TagNone);
	box = gameObjectManager_->CreateInstance<SampleBox>("box", BaseObject::TagNone);
}

void TitleScene::Update()
{
	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}
