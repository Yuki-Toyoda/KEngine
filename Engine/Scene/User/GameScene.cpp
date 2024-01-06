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

	// 追従カメラ生成
	FollowCamera* camera = nullptr;
	camera = gameObjectManager_->CreateInstance<FollowCamera>("MainCamera", BaseObject::TagCamera);
	camera->SetTarget(&player->transform_);
	camera->UseThisCamera();
	player->followCamera_ = camera;

	// 床生成
	Floor* floor = nullptr;
	floor = gameObjectManager_->CreateInstance<Floor>("Floor", BaseObject::TagFloor);
	floor->transform_.scale_ = { 100.0f, 1.0f, 100.0f };
	floor->transform_.translate_.y = -1.0f;

	// 敵生成
	Enemy* enemy = nullptr;
	enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
	enemy->transform_.translate_ = { 0.0f, 2.0f, 10.0f };

	// ロックオンクラス生成
	LockOn* lockOn = nullptr;
	lockOn = gameObjectManager_->CreateInstance<LockOn>("LockOn", BaseObject::TagPlayer);
	lockOn->SetCamera(camera);
	camera->SetLockOn(lockOn);
	player->lockOn_ = lockOn;
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
