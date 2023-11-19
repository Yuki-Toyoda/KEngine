#include "GameScene.h"
#include "../SceneManager.h"

void GameScene::Initialize(){

	Camera* newcamera = nullptr;
	newcamera = gameObjectManager_->CreateInstance<Camera>("mainCamera", BaseObject::TagCamera);
	newcamera->UseThisCamera();

	/*SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);*/

	SampleBox* box = nullptr;
	box = gameObjectManager_->CreateInstance<SampleBox>("box", BaseObject::TagNone);
}

void GameScene::Update()
{
	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}
