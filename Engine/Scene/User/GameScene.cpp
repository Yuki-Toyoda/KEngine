#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){
	InputManager::Init();
	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	player_ = nullptr;
	player_ = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);
	weapon_ = nullptr;
	weapon_ = gameObjectManager_->CreateInstance<Weapon>("wepon", BaseObject::TagWeapon);
	weapon_->SetTarget(&player_->transform_);
	weapon_->transform_.SetParent(&player_->transform_, 0b001);

	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);

	Enemy* enemy ;
	for (int i = 0; i < 15; i++) {
		enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
		
		enemy->SetWepon(weapon_);
		enemy->SetStartTransform();
		enemy->isActive_ = false;
		enemies_.push_back(enemy);
	}
	
	chain_ = gameObjectManager_->CreateInstance<Chain>("chain", BaseObject::TagChain);
	chain_->SetPlayer(player_);
	chain_->SetWepon(weapon_);
	/*Obstacle* obstacle;
	for (int i = 0; i < 5; i++) {
		obstacle = gameObjectManager_->CreateInstance<Obstacle>("obstacle", BaseObject::TagObstacle);
		obstacle->transform_.translate_.x = -8.0f;
		obstacles_.push_back(obstacle);
	}*/
	MidEnemy* midenemy;
	for (int i = 0; i < 3; i++) {
		midenemy = gameObjectManager_->CreateInstance<MidEnemy>("midEnemy", BaseObject::TagEnemy);
	
		midenemy->SetWepon(weapon_);
		midEnemies_.push_back(midenemy);
	}
	/*obstacles_[0]->transform_.translate_ = { 10.0f,6.0f,0.0f };
	obstacles_[1]->transform_.translate_ = { -10.0f,-6.0f,0.0f };
	obstacles_[2]->transform_.translate_ = { 18.0f,-16.0f,0.0f };
	obstacles_[3]->transform_.translate_ = { -18.0f,6.0f,0.0f };
	obstacles_[4]->transform_.translate_ = { 15.0f,18.0f,0.0f };*/
	HighEnemy* highenemy;
	for (int i = 0; i < 3; i++) {
		highenemy = gameObjectManager_->CreateInstance<HighEnemy>("highEnemy", BaseObject::TagEnemy);
		
		highenemy->SetWepon(weapon_);
		highEnemies_.push_back(highenemy);
	}
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;
	//ground_ = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	// UIマネージャの生成
	gameObjectManager_->CreateInstance<InGameUIManager>("UIManager", BaseObject::TagNone);

}

void GameScene::Update()
{
	InputManager::Update();
	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
	if (input_->TriggerKey(DIK_LSHIFT)) {
		BaseScene* nextScene = new EditorScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
	if (weapon_->GetIsAtackEnd()) {
		camera_->Shake();
	}
	if (weapon_->GetISBreak()||weapon_->GetIsAtackEnd()&&!camera_->GetIsShake()) {
		//ground_->Damage(weapon_);
		AtackAfterInit();
	}
}

void GameScene::AtackAfterInit()
{
	
	//wepon = gameObjectManager_->CreateInstance<Weapon>("wepon", BaseObject::TagWeapon);
	weapon_->SetTarget(&player_->transform_);
	weapon_->Reset();
	weapon_->transform_.SetParent(&player_->transform_, 0b001);
	
	chain_->SetWepon(weapon_);
	for (Enemy* enemy : enemies_) {
		enemy->Reset();
		enemy->SetWepon(weapon_);
	}
	for (MidEnemy* enemy : midEnemies_) {
		enemy->Reset();
		enemy->SetWepon(weapon_);
	}
	for (HighEnemy* enemy : highEnemies_) {
		enemy->Reset();
		enemy->SetWepon(weapon_);
	}
	
	Enemy* enemy;
	for (int i = 0; i < 15; i++) {
		
		enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
		enemy->transform_.translate_.x = i * 2.0f;
		enemy->SetWepon(weapon_);
		enemies_.push_back(enemy);
	}
}
