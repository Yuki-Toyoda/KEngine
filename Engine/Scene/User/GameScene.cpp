#include "GameScene.h"
#include "../SceneManager.h"

void GameScene::Initialize(){

	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤー生成
	SamplePlayer* player = nullptr;
	player = gameObjectManager_->CreateInstance<SamplePlayer>("Player", BaseObject::TagPlayer);
	player->transform_.translate_ = { 0.0f, 3.0f, 0.0f };

	// 床生成
	SampleFloor* floor = nullptr;
	floor = gameObjectManager_->CreateInstance<SampleFloor>("Floor", BaseObject::TagFloor);
	floor->transform_.scale_ = { 50.0f, 1.0f, 50.0f };

	// 追従カメラ生成
	ThirdPersonCamera* camera = nullptr;
	camera = gameObjectManager_->CreateInstance<ThirdPersonCamera>("TPCamera", BaseObject::TagCamera);
	camera->SetTarget(&player->transform_);
	player->SetTPCamera(camera);
	camera->UseThisCamera();

	// 武器生成
	SampleWeapon* weapon = nullptr;
	weapon = gameObjectManager_->CreateInstance<SampleWeapon>("Weapon", BaseObject::TagWeapon);
	weapon->SetTarget(&player->transform_);
	player->SetWeapon(weapon);

	// 敵生成
	SampleEnemy* enemy = nullptr;
	for (int i = 0; i < 3; i++) {
		enemy = gameObjectManager_->CreateInstance<SampleEnemy>("Enemy", BaseObject::TagEnemy);
		enemy->transform_.translate_ = { Math::RandomF(-15.0f, 15.0f, 2), 0.0f, Math::RandomF(10.0f, 15.0f, 2) };
	}

	// ロックオンシステム生成
	LockOn* lockOn = nullptr;
	lockOn = gameObjectManager_->CreateInstance<LockOn>("LockOn", BaseObject::TagPlayer);
	player->SetLockOn(lockOn);
	lockOn->SetTPCamera(camera);
	camera->SetLockOn(*lockOn);
}

void GameScene::Update()
{
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

	ImGui::Begin("particle");
	if (ImGui::Button("Play")) {
		// ロックオンシステム生成
		SampleHitParticle* hitParticle = nullptr;
		hitParticle = gameObjectManager_->CreateInstance<SampleHitParticle>("HitParticle", BaseObject::TagPlayer);
	}
	ImGui::End();

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG

	

	
#endif // _DEBUG
}
