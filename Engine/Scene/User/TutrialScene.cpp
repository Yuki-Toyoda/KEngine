#include "TutrialScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"
#include "../SceneList.h"
void TutrialScene::Init()
{
	// 入力マネージャの初期化
	InputManager::Init();
	GameManager* gameManager = nullptr;

	// 音再生クラスのインスタンス取得
	audio_ = Audio::GetInstance();

	// BGMロード
	// BGMロード
	bgmHandle_ = audio_->LoadWave("./Resources/Audio/BGM/tutorial.wav");
	bgmVoiceHadle_ = audio_->PlayWave(bgmHandle_, true, 1.0f);

	gameManager = gameObjectManager_->CreateInstance<GameManager>("gameManager", BaseObject::TagNone);
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	TutrialCamera* tcamera = nullptr;
	tcamera = gameObjectManager_->CreateInstance<TutrialCamera>("TutrialCamera", BaseObject::TagCamera);
	// スカイドーム生成
	SkyDome* skyDome = nullptr;
	skyDome = gameObjectManager_->CreateInstance<SkyDome>("SkyDome", BaseObject::TagNone);
	// 地面生成
	Ground* ground;
	ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	ground->transform_.scale_ = { 55.0f,1.0f,55.0f };
	// プレイヤー生成
	player_ = nullptr;
	player_ = gameObjectManager_->CreateInstance<Player>("Player", BaseObject::TagPlayer);
	// プレイヤーのy座標
	player_->transform_.scale_ = { 2.0f, 2.0f, 2.0f };
	player_->transform_.translate_ = { 0.0f,3.0f,-40.0f };

	player_->SetIsTutrial(true);
	player_->SetIsTutrialDash(false);
	player_->SetCanDash(false);
	// プレイヤーをカメラにセットする
	camera_->SetPlayer(player_);
	// プレイヤーにカメラをセットする
	player_->SetCamera(camera_);

	// ボスの生成
	boss_ = gameObjectManager_->CreateInstance<Boss>("Boss", BaseObject::TagEnemy);
	// ボスにプレイヤーをセット
	boss_->SetPlayer(player_);
	// ボスにインゲームカメラをセットする
	boss_->SetInGameCamera(camera_);
	//GameManagerをセット
	boss_->SetgameManager(gameManager);
	player_->SetBoss(boss_);
	boss_->SetIsTutrial(true);
	// ウリボの生成
	uribo_ = gameObjectManager_->CreateInstance<Uribo>("uribo", BaseObject::TagUribo);
	uribo_->SetTutrial(true);
	uribo_->SetTutrialStart(false);

	player_->SetUribo(uribo_);
	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);
	// プレイヤーに自身を渡す
	player_->SetPlayerAnimManager(am);
	// アニメーションを生成
	am->CreateAnimation();

	
	// プレイヤーに生成した地面をセット
	player_->SetGround(ground);
	
	
	// ボスのアニメーションマネージャーの生成
	BossAnimManager* bam = gameObjectManager_->CreateInstance<BossAnimManager>("bossAnim", BaseObject::TagEnemy);
	// アニメーションマネージャーにボスをセット
	bam->SetBoss(boss_);
	// アニメーション生成
	bam->CreateAnimation();

	// ボスにボスのアニメーションマネージャーをセット
	boss_->SetBossAnimManager(bam);

	// 柵の生成
	Fences* fm = gameObjectManager_->CreateInstance<Fences>("Fence", BaseObject::TagNone);
	// 柵追加
	fm->Add(Vector3(0.0f, 3.5f, ground->transform_.scale_.z), Vector3(0.0f, 0.0f, 0.0f), Vector2(ground->transform_.scale_.x, 2.5f));
	fm->Add(Vector3(0.0f, 3.5f, -ground->transform_.scale_.z), Vector3(0.0f, 0.0f, 0.0f), Vector2(ground->transform_.scale_.x, 2.5f));
	fm->Add(Vector3(ground->transform_.scale_.x, 3.5f, 0.0f), Vector3(0.0f, (float)(std::numbers::pi / 2.0f), 0.0f), Vector2(ground->transform_.scale_.z, 2.5f));
	fm->Add(Vector3(-ground->transform_.scale_.x, 3.5f, 0.0f), Vector3(0.0f, (float)(std::numbers::pi / 2.0f), 0.0f), Vector2(ground->transform_.scale_.z, 2.5f));

	// UIマネージャの生成
	InGameUIManager* iUIm = gameObjectManager_->CreateInstance<InGameUIManager>("UIManager", BaseObject::TagNone);
	// UIマネージャーにプレイヤーをセット
	iUIm->SetPlayer(player_);
	// UIマネージャーにボスをセット
	iUIm->SetBoss(boss_);
	// UIマネージャーにウリボーをセット
	iUIm->SetUribo(uribo_);
	iUIm->SetIsTutrial(true);
	// UIマネージャーにカメラをセット
	iUIm->SetCamera(camera_);
	 tm_ = gameObjectManager_->CreateInstance<TutrialManager>("tutrialManager", BaseObject::TagNone);

	tm_->SetBoss(boss_);
	tm_->SetPlayer(player_);
	tm_->SetUribo(uribo_);
	tm_->SetIngameCamera(camera_);
	tm_->SetTutrialCamera(tcamera);

	// チュートリアルマネージャーにインゲームUIマネージャーをセット
	tm_->SetInGameUIManger(iUIm);

	// フェードイン
	FadeManager::GetInstance()->ChangeParameter("FadeIn", true);
	FadeManager::GetInstance()->Play();
	boss_->ChangeState(std::make_unique<TutrialState>());
}

void TutrialScene::Update()
{
	InputManager::Update();
	if (tm_->GetTutrialEnd()) {
		// BGMを止める
		audio_->StopWave(bgmVoiceHadle_);
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

	// デバッグ遷移
	/*if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}*/

#ifdef _DEBUG

	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {

		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);

		// BGMを止める
		audio_->StopWave(bgmVoiceHadle_);
	}

#endif // _DEBUG

}
