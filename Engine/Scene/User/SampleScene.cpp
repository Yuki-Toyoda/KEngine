#include "SampleScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"
#include "../../Level/LevelLoader.h"

void SampleScene::LoadModel()
{
	// モデルデータマネージャーを使用して事前ロードを行ってください
	// modelDataManager_->LoadNewModel("モデルまでのファイルパス", "モデル名.拡張子");

	// 新規モデル読み込み処理
}

void SampleScene::Init()
{
	// スカイボックスの初期化
	skyBox_->Init("./Engine/Resource/Samples/Texture", "rostock_laage_airport_4k.dds");

	// オブジェクトの生成
	gameObjectManager_->CreateInstance<Camera>("Camera", IObject::TagCamera);
	//gameObjectManager_->CreateInstance<SkyDome>("SkyDome", IObject::TagNone);
	//gameObjectManager_->CreateInstance<TestObject>("Test", IObject::TagNone);
	gameObjectManager_->CreateInstance<SampleAnim>("Test", IObject::TagNone);

}

void SampleScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("");
	}
}
