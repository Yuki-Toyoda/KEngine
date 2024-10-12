#include "KEngine.h"

void KEngineFrameWork::Init()
{
	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// ウィンドウズアプリケーションクラスのインスタンスを取得
	winApp_ = WinApp::GetInstance();
	// ゲームウィンドウを生成する
	winApp_->CreateGameWindow(L"KEngine", WS_OVERLAPPEDWINDOW, KEngine::Config::Window::KWindowWidth, KEngine::Config::Window::KWindowHeight);

	// DirectX汎用クラスのインスタンスを取得
	dxCommon_ = DirectXCommon::GetInstance();
	// DirectXの初期化
	dxCommon_->Init(winApp_);

	// 入力の初期化
	input_ = Input::GetInstance();
	input_->Init();

	// オーディオの初期化
	audio_ = Audio::GetInstance();
	audio_->Init();

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	// ImGuiの初期化
	imGuiManager_ = ImGuiManager::GetImstance();
	imGuiManager_->Intialize(winApp_, dxCommon_);
	// DockingImGuiの有効化
	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// 衝突マネージャーの初期化
	collisionManager_ = CollisionManager::GetInstance();

	// アニメーションマネージャの初期化
	animManager_ = AnimationManager::GetInstance();
	animManager_->Init();

	// オブジェクトマネージャーの初期化
	gameObjectManager_ = GameObjectManager::GetInstance();
	gameObjectManager_->Init();

	// スプライトマネージャの初期化
	spriteManager_ = SpriteManager::GetInstance();
	spriteManager_->Init();

	// パーティクルマネージャーの取得
	particleManager_ = ParticleManager::GetInstance();

	// シーンマネージャーのインスタンス取得
	sceneManager_ = SceneManager::GetInstance();
}

void KEngineFrameWork::Update()
{
	// ゲームが終了するメッセージを受けとったら
	if (winApp_->ProcessMessage()) {
		// ゲーム終了
		endGameRequest_ = true;
	}

	// ImGui受付開始
	imGuiManager_->Begin();

	// 入力関連の毎フレーム処理
	input_->Update();

	// 衝突判定リストクリア
	collisionManager_->ListClear();

	// グローバル変数の更新
	GlobalVariables::GetInstance()->Update();

	// シーンマネージャー更新
	sceneManager_->Update();

	#ifdef _DEBUG // デバッグのみで実行

	// シーンマネージャーのImGuiを表示
	sceneManager_->DisplayImGui();

	#endif // _DEBUG

	// オブジェクトマネージャー更新
	gameObjectManager_->Update();

	// スプライトマネージャ更新
	spriteManager_->Update();

	// アニメーションマネージャ更新
	animManager_->Update();
	animManager_->DisplayImGui();

	// 衝突判定検証
	collisionManager_->CheckAllCollision();
}

void KEngineFrameWork::Finalize()
{	
	// 衝突マネージャーリストクリア
	collisionManager_->ListClear();

	// 全オブジェクトを削除
	gameObjectManager_->Init();
	// 読み込みスプライト削除
	spriteManager_->Init();
	// 全アニメーション削除
	animManager_->Init();

	// 全パーティクル削除
	particleManager_->Clear();

	// 音解放処理
	audio_->Finalize();

	// ImGui解放
	imGuiManager_->Finalize();

	// ゲームウィンドウを閉じる
	winApp_->TerminateGameWindow();

	// COM終了
	CoUninitialize();
}

void KEngineFrameWork::Run()
{
	// ゲームの初期化を行う
	Init();
	// シーンマネージャーはゲーム固有の初期化が終わってから初期化
	sceneManager_->Init();

	// メインループ
	while (true) {
		
		// ゲームの更新処理を呼び出す
		Update();

		// 終了リクエストを受け取った場合はゲームを終了させる
		if (GetIsEndGameRequest()) {
			break;
		}

		// ゲームの描画処理を呼び出す
		Draw();
	}

	// ゲームの終了処理を行う
	Finalize();
}
