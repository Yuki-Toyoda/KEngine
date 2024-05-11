#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", IObject::TagNone);

	// プレイヤー追加
	player_ = gameObjectManager_->CreateInstance<Player>("Player", IObject::TagPlayer);

	// 追従カメラ生成
	FollowCamera* camera = nullptr;
	camera = gameObjectManager_->CreateInstance<FollowCamera>("MainCamera", IObject::TagCamera);
	camera->SetTarget(&player_->transform_);
	camera->UseThisCamera();
	player_->followCamera_ = camera;

	// 床生成
	Floor* floor = nullptr;
	floor = gameObjectManager_->CreateInstance<Floor>("Floor", IObject::TagFloor);
	floor->transform_.scale_ = { 100.0f, 1.0f, 100.0f };
	floor->transform_.translate_.y = -1.0f;

	// 敵生成
	enemy_ = gameObjectManager_->CreateInstance<Enemy>("Enemy", IObject::TagEnemy);
	enemy_->transform_.translate_ = { 0.0f, 1.5f, 10.0f };
	enemy_->SetPlayerPos(&player_->transform_);
	player_->enemy_ = this->enemy_;
	enemy_->player_ = this->player_;

	// ロックオンクラス生成
	LockOn* lockOn = nullptr;
	lockOn = gameObjectManager_->CreateInstance<LockOn>("LockOn", IObject::TagPlayer);
	lockOn->SetCamera(camera);
	camera->SetLockOn(lockOn);
	player_->lockOn_ = lockOn;

	// ナビィ生成
	Fairy* fairy = nullptr;
	fairy = gameObjectManager_->CreateInstance<Fairy>("Fairy", IObject::TagPlayer);
	fairy->SetTarget(&player_->transform_);
	fairy->SetPlayer(player_);
}

void GameScene::Update()
{
	if (enemy_->isDead_ || player_->isDead_) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
#endif // _DEBUG
}
