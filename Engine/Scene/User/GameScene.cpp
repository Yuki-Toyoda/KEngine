#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"

void GameScene::Init(){

	// 入力マネージャの初期化
	InputManager::Init();
	
	// カメラの生成
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
	// プレイヤーのy座標
	player_->transform_.scale_ = { 2.0f, 2.0f, 2.0f };
	player_->transform_.translate_.y = 3.0f;
	player_->transform_.translate_.x = 10.0f;
	//// 敵を生成
	//gameObjectManager_->CreateInstance<SmallEnemy>("Enemy", BaseObject::TagEnemy);

	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);
	// プレイヤーに自身を渡す
	player_->SetPlayerAnimManager(am);
	// アニメーションを生成
	am->CrateAnimation();

	// 地面生成
	Ground* ground;
	ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	ground->transform_.scale_ = { 55.81f,1.0f,32.5f };
	// プレイヤーに生成した地面をセット
	player_->SetGround(ground);

	// ボスの生成
	Boss* boss;
	boss = gameObjectManager_->CreateInstance<Boss>("Boss", BaseObject::TagEnemy);
	// ボスにプレイヤーをセット
	boss->SetPlayer(player_);
	
	// 柵の生成
	Fences* fm = gameObjectManager_->CreateInstance<Fences>("Fence", BaseObject::TagNone);
	// 柵追加
	fm->Add(Vector3(0.0f, 3.5f, ground->transform_.scale_.z), Vector3(0.0f, 0.0f, 0.0f), Vector2(ground->transform_.scale_.x, 2.5f));
	fm->Add(Vector3(0.0f, 3.5f, -ground->transform_.scale_.z), Vector3(0.0f, 0.0f, 0.0f), Vector2(ground->transform_.scale_.x, 2.5f));
	fm->Add(Vector3(ground->transform_.scale_.x, 3.5f, 0.0f), Vector3(0.0f, (float)(std::numbers::pi / 2.0f), 0.0f), Vector2(ground->transform_.scale_.z, 2.5f));
	fm->Add(Vector3(-ground->transform_.scale_.x, 3.5f, 0.0f), Vector3(0.0f, (float)(std::numbers::pi / 2.0f), 0.0f), Vector2(ground->transform_.scale_.z, 2.5f));

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
		BaseScene* nextScene = new ResultScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
	if (input_->TriggerKey(DIK_LSHIFT)) {
		BaseScene* nextScene = new EditorScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
	
}