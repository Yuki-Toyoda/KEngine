#include "SampleScene.h"
#include "../../GameObject/SampleList.h"

void SampleScene::Init()
{
	// オブジェクトの生成
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
	gameObjectManager_->CreateInstance<TestObject>("Test", BaseObject::TagNone);
}

void SampleScene::Update()
{
	
}
