#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	Player* player = nullptr;
	player = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);

	Wepon* wepon = nullptr;
	wepon = gameObjectManager_->CreateInstance<Wepon>("wepon", BaseObject::TagPlayer);
	wepon->SetTarget(&player->transform_);
	wepon->transform_.SetParent(&player->transform_, 0b001);
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
