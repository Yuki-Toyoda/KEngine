#include "SampleScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", IObject::TagCamera);
	gameObjectManager_->CreateInstance<SkyDome>("SkyDome", IObject::TagNone);
	gameObjectManager_->CreateInstance<TestObject>("Test", IObject::TagNone);
	gameObjectManager_->CreateInstance<SampleAnim>("Test", IObject::TagNone);

}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
