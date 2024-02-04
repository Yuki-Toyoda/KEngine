#include "SampleScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", BaseObject::TagCamera);
	TestObject* test1 = gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	test1->transform_.translate_.x = { -1.5f };
	TestObject* test2 = gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	test2->transform_.translate_.x = { 1.5f };
}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
