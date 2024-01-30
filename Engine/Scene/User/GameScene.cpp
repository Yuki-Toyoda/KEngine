#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤー追加
	player_ = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);

	// 追従カメラ生成
	FollowCamera* camera = nullptr;
	camera = gameObjectManager_->CreateInstance<FollowCamera>("MainCamera", BaseObject::TagCamera);
	camera->SetTarget(&player_->transform_);
	camera->UseThisCamera();
	player_->followCamera_ = camera;

	// 床生成
	Floor* floor = nullptr;
	floor = gameObjectManager_->CreateInstance<Floor>("Floor", BaseObject::TagFloor);
	floor->transform_.scale_ = { 100.0f, 1.0f, 100.0f };
	floor->transform_.translate_.y = -1.0f;

	// 敵生成
	enemy_ = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
	enemy_->transform_.translate_ = { 0.0f, 1.5f, 10.0f };
	enemy_->SetPlayerPos(&player_->transform_);
	player_->enemy_ = this->enemy_;
	enemy_->player_ = this->player_;

	// ロックオンクラス生成
	LockOn* lockOn = nullptr;
	lockOn = gameObjectManager_->CreateInstance<LockOn>("LockOn", BaseObject::TagPlayer);
	lockOn->SetCamera(camera);
	camera->SetLockOn(lockOn);
	player_->lockOn_ = lockOn;
}

void GameScene::Update()
{
	// デバッグ遷移
	if (enemy_->isDead_ || player_->isDead_) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}
