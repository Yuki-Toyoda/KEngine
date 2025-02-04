#include "SampleScene.h"
#include "Engine/GameObject/SampleList.h"
#include "Engine/Scene/SceneManager.h"

void SampleScene::Init()
{
	// スカイボックスの初期化
	skyBox_->Init("./Engine/Resource/System/Texture", "rostock_laage_airport_4k.dds");

	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", IObject::TagCamera);
	gameObjectManager_->CreateInstance<TestObject>("Test", IObject::TagNone);
	gameObjectManager_->CreateInstance<TestObject>("Test", IObject::TagNone);
}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
