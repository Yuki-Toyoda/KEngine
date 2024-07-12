#include "GameScene.h"
#include "../SceneManager.h"
#include "../../Level/LevelLoader.h"

void GameScene::LoadModel()
{
	// モデルデータマネージャーを使用して事前ロードを行ってください
	// modelDataManager_->LoadNewModel("モデルまでのファイルパス", "モデル名.拡張子");

	// 新規モデル読み込み処理
	modelDataManager_->LoadNewModel("./Engine/Resource/Samples/Sphere", "Sphere.obj");
}

void GameScene::Init(){
	// プレイヤー追加
	player_ = gameObjectManager_->CreateInstance<Player>("Player", IObject::TagPlayer);

	// 追従カメラ生成
	FollowCamera* camera = nullptr;
	camera = gameObjectManager_->CreateInstance<FollowCamera>("MainCamera", IObject::TagCamera);
	camera->SetTarget(&player_->transform_);
	camera->UseThisCamera();
	player_->followCamera_ = camera;

	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", IObject::TagNone);

	level_ = gameObjectManager_->CreateInstance<SampleLevelObjects>("Level", IObject::TagNone);
	level_->LoadLevel("./Resources/Level", "Stage.json");

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

	// Rキーを押すとステージをリロード
	if (input_->TriggerKey(DIK_R)) {
		level_->LoadLevel("./Resources/Level", "Stage.json");
	}

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
#endif // _DEBUG
}
