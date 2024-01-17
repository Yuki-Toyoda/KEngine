#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// 入力マネージャの初期化
	InputManager::Init();
	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤー生成
	player_ = nullptr;
	player_ = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);
	// 武器生成
	weapon_ = nullptr;
	weapon_ = gameObjectManager_->CreateInstance<Weapon>("wepon", BaseObject::TagWeapon);
	weapon_->SetTarget(&player_->transform_);
	weapon_->transform_.SetParent(&player_->transform_, 0b001);

	// 敵を生成
	gameObjectManager_->CreateInstance<SmallEnemy>("Enemy", BaseObject::TagEnemy);

	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);
	
	// 鎖生成
	chain_ = gameObjectManager_->CreateInstance<Chain>("chain", BaseObject::TagChain);
	chain_->SetPlayer(player_);
	chain_->SetWepon(weapon_);

	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;

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
	/*for (Enemy* enemy : enemies_) {
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
	}*/
}
