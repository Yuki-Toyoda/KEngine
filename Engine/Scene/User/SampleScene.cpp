#include "SampleScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", BaseObject::TagCamera);

	// テストオブジェクトの生成
	TestObject* test = gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagPlayer);
	test->transform_.translate_.x = -1.5f;
	test = gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	test->transform_.translate_.x = 1.5f;
}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
