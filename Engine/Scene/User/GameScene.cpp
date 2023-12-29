#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤー追加
	Player* player = nullptr;
	player = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);

	FollowCamera* camera = nullptr;
	camera = gameObjectManager_->CreateInstance<FollowCamera>("MainCamera", BaseObject::TagCamera);
	camera->SetTarget(&player->transform_);
	camera->UseThisCamera();
	player->followCamera_ = camera;

	Floor* floor = nullptr;
	floor = gameObjectManager_->CreateInstance<Floor>("Floor", BaseObject::TagFloor);
	floor->transform_.scale_ = { 100.0f, 0.35f, 100.0f };
	floor->transform_.translate_.y = -0.35f;
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
