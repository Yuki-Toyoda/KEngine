#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/SampleList.h"
#include "../../Particle/ParticleEmitterManager.h"

void GameScene::Init(){

	// 入力マネージャの初期化
	InputManager::Init();
	GameManager* gameManager = nullptr;
	
	// 音再生クラスのインスタンス取得
	audio_ = Audio::GetInstance();

	// BGMロード
	bgmHandle_ = audio_->LoadWave("./Resources/Audio/BGM/inGame.wav");

	gameManager = gameObjectManager_->CreateInstance<GameManager>("gameManager", BaseObject::TagNone);
	// カメラの生成
	camera_ = nullptr;
	camera_ = gameObjectManager_->CreateInstance<InGameCamera>("Incamera", BaseObject::TagCamera);
	camera_->UseThisCamera();
	//camera_->fov_ = 0.85f;
	//camera_->transform_.translate_ = { 0.0f,47.0f,-90.0f };
	//camera_->transform_.rotate_ = { 0.55f,0.0f,0.0f };
	
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
	uribo_ = gameObjectManager_->CreateInstance<Uribo>("uribo", BaseObject::TagUribo);
	player_->SetUribo(uribo_);
	// プレイヤーアニメーションマネージャの生成
	PlayerAnimManager* am = gameObjectManager_->CreateInstance<PlayerAnimManager>("playerAnim", BaseObject::TagPlayer);
	// プレイヤーを渡す
	am->SetPlayer(player_);
	// プレイヤーに自身を渡す
	player_->SetPlayerAnimManager(am);
	// アニメーションを生成
	am->CreateAnimation();

	// 地面生成
	Ground* ground;
	ground = gameObjectManager_->CreateInstance<Ground>("Ground", BaseObject::TagFloor);
	ground->transform_.scale_ = { 55.0f,1.0f,55.0f };
	// プレイヤーに生成した地面をセット
	player_->SetGround(ground);

	// ボスの生成
	boss_ = gameObjectManager_->CreateInstance<Boss>("Boss", BaseObject::TagEnemy);
	// ボスにプレイヤーをセット
	boss_->SetPlayer(player_);
	//GameManagerをセット
	boss_->SetgameManager(gameManager);

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

	// フェードイン
	FadeManager::GetInstance()->ChangeParameter("FadeIn", true);
	FadeManager::GetInstance()->Play();

	//ParticleEmitterManager::GetInstance()->CreateEmitter<IParticleEmitter, IParticle>("ParticleTest", 100, 4, { 0.0f,0.0f,0.0f }, 100, 2, TextureManager::Load("uvChecker.png"));

}

void GameScene::Update()
{
	// 再生されていなければ再生する
	if (!audio_->IsPlaying(bgmVoiceHadle_) || bgmVoiceHadle_ == -1) {
		bgmVoiceHadle_ = audio_->PlayWave(bgmHandle_, false, 1.0f);
	}

	// BGM音量を設定
	//audio_->SetVolume(bgmHandle_, FadeManager::GetInstance()->GetVolume());


	// ボスのHPが0以下の時
	if (boss_->GetHP() <= 0) {
		// ボスが死亡したことを伝える
		uribo_->SetIsBossDead(true);
	}

	InputManager::Update();
	if (player_->GetgameOver()) {
		// クリアフラグをfalse
		ResultScene::isClear_ = false;
		BaseScene* nextScene = new ResultScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);

		// BGMを止める
		audio_->StopWave(bgmVoiceHadle_);
	}
	if (boss_->GetIsSceneChage()){
		// クリアフラグをtrue
		ResultScene::isClear_ = true;
		BaseScene * nextScene = new ResultScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);

		// BGMを止める
		audio_->StopWave(bgmVoiceHadle_);
	}
	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new ResultScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);

		// BGMを止める
		audio_->StopWave(bgmVoiceHadle_);
	}
	if (input_->TriggerKey(DIK_LSHIFT)) {
		BaseScene* nextScene = new EditorScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);

		// BGMを止める
		audio_->StopWave(bgmVoiceHadle_);
	}
#endif // _DEBUG
	
}