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
	wepon_ = nullptr;
	wepon_ = gameObjectManager_->CreateInstance<Wepon>("wepon", BaseObject::TagWeapon);
	wepon_->SetTarget(&player_->transform_);
	wepon_->transform_.SetParent(&player_->transform_, 0b001);

	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);

	Enemy* enemy ;
	for (int i = 0; i < 10; i++) {
		enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
		enemy->transform_.translate_.x = i * 2.0f;
		enemy->SetWepon(wepon_);
		enemy->SetStartTransform();
		enemies_.push_back(enemy);
	}
	
	chain_ = gameObjectManager_->CreateInstance<Chain>("chain", BaseObject::TagChain);
	chain_->SetPlayer(player_);
	chain_->SetWepon(wepon_);
	Obstacle* obstacle;
	for (int i = 0; i < 5; i++) {
		obstacle = gameObjectManager_->CreateInstance<Obstacle>("obstacle", BaseObject::TagObstacle);
		obstacle->transform_.translate_.x = -8.0f;
		obstacles_.push_back(obstacle);
	}
	obstacles_[0]->transform_.translate_ = { 10.0f,6.0f,0.0f };
	obstacles_[1]->transform_.translate_ = { -10.0f,-6.0f,0.0f };
	obstacles_[2]->transform_.translate_ = { 18.0f,-16.0f,0.0f };
	obstacles_[3]->transform_.translate_ = { -18.0f,6.0f,0.0f };
	obstacles_[4]->transform_.translate_ = { 15.0f,18.0f,0.0f };
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
#endif // _DEBUG
	if (wepon_->GetIsAtackEnd()) {
		camera_->Shake();
	}
	if (wepon_->GetISBreak()||wepon_->GetIsAtackEnd()&&!camera_->GetIsShake()) {
		AtackAfterInit();
	}
}

void GameScene::AtackAfterInit()
{
	
	//wepon = gameObjectManager_->CreateInstance<Wepon>("wepon", BaseObject::TagWeapon);
	wepon_->SetTarget(&player_->transform_);

	wepon_->Reset();
	wepon_->transform_.SetParent(&player_->transform_, 0b001);
	
	chain_->SetWepon(wepon_);
	for (Enemy* enemy : enemies_) {
		enemy->Reset();
		enemy->SetWepon(wepon_);
	}
	/*enemies_.remove_if([](Enemy* enemy) {
		delete enemy;
		return true;
		});
	Enemy* enemy;
	for (int i = 0; i < 15; i++) {
		
		enemy = gameObjectManager_->CreateInstance<Enemy>("Enemy", BaseObject::TagEnemy);
		enemy->transform_.translate_.x = i * 2.0f;
		enemy->SetWepon(wepon_);
		enemies_.push_back(enemy);
	}*/
}
