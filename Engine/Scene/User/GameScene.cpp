#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// 入力マネージャの初期化
	InputManager::Init();
	
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	camera_->fov_ = 0.85f;
	camera_->transform_.translate_ = { 0.0f,47.0f,-85.0f };
	camera_->transform_.rotate_ = { 0.55f,0.0f,0.0f };
	
	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);

	// プレイヤー生成
	player_ = nullptr;
	player_ = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);
	player_->transform_.translate_.y = 2.0f;
	
	//// 敵を生成
	//gameObjectManager_->CreateInstance<SmallEnemy>("Enemy", BaseObject::TagEnemy);

	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);
	Ground* ground;
	ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	ground->transform_.scale_ = { 55.81f,1.0f,32.5f };
	player_->SetGround(ground);
	Boss* boss;
	boss = gameObjectManager_->CreateInstance<Boss>("Boss", BaseObject::TagEnemy);
	boss->SetPlayer(player_);
	
	// UIマネージャの生成
	gameObjectManager_->CreateInstance<InGameUIManager>("UIManager", BaseObject::TagNone);

	// フェードイン
	FadeManager::GetInstance()->ChangeParameter("FadeIn", true);
	FadeManager::GetInstance()->Play();
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
	
}