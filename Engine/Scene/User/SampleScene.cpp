#include "SampleScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", BaseObject::TagCamera);
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);

}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
