#include "GameScene.h"
#include "../../GameObject/SampleList.h"
#include "../SceneManager.h"

void GameScene::Initialize() {
	// 環境生成
	Ambient* ambient = new Ambient(); // インスタンス生成
	ambient->Initialize("ambient", BaseObject::tagOther); // 初期化
	gameObjectManager_->AddGameObject(ambient); // ゲームオブジェクトマネージャーに追加

	// 床生成
	SampleFloor* floor = new SampleFloor(); // インスタンス生成
	floor->Initialize("floor", BaseObject::tagFloor); // 初期化
	floor->transform_.scale_ = { 50.0f, 0.5f, 50.0f }; // スケールを設定
	gameObjectManager_->AddGameObject(floor); // マネージャーに追加

	// プレイヤー生成
	SamplePlayer* player = new SamplePlayer(); // インスタンス生成
	player->Initialize("player", BaseObject::tagPlayer); // 初期化
	gameObjectManager_->AddGameObject(player); // マネージャーに追加

	// カメラ生成
	TPCamera* camera = new TPCamera(); // インスタンス生成
	camera->Initialize("mainCamera", BaseObject::tagCamera); // 初期化
	camera->SetTarget(&player->transform_); // 追従ターゲットをプレイヤーに
	gameObjectManager_->AddGameObject(camera); // マネージャーに追加
	camera->UseThisCamera(); // 生成したカメラを使用する
	player->SetTPCamera(camera); // 追従カメラをプレイヤーにセット

	// 武器生成
	Weapon* weapon = new Weapon(); // インスタンス生成
	weapon->Initialize("weapon", BaseObject::tagWeapon); // 初期化
	gameObjectManager_->AddGameObject(weapon); // マネージャーに追加
	player->SetWeapon(weapon); // 武器をセット
	weapon->SetTarget(&player->transform_);

	// フェードイン
	SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 1.0f });
	SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f, 0.0f },false);
}

void GameScene::Update() {
#ifdef _DEBUG
	// デバッグ遷移
	if (Input::GetInstance()->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->SetFadeColor({ 0.0f, 0.0f, 0.0f, 0.0f });
		SceneManager::GetInstance()->StartFadeEffect(1.0f, { 0.0f, 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f, 1.0f });
		BaseScene* nextScene = new TitleScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}