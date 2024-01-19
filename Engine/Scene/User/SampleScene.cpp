#include "SampleScene.h"
#include "../../GameObject/SampleList.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", BaseObject::TagCamera);
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagPlayer);
}

void SampleScene::Update()
{
	
}
