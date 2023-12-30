#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){
	
	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	player_ = nullptr;
	player_ = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);
	wepon_ = nullptr;
	wepon_ = gameObjectManager_->CreateInstance<Wepon>("wepon", BaseObject::TagWeapon);
	wepon_->SetTarget(&player_->transform_);
	wepon_->transform_.SetParent(&player_->transform_, 0b001);
	Enemy* enemy ;
	for (int i = 0; i < 15; i++) {
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
	obstacle = gameObjectManager_->CreateInstance<Obstacle>("obstacle", BaseObject::TagObstacle);
	obstacle->transform_.translate_.x = -8.0f;
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
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
