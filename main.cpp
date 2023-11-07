#include <Windows.h>
#include "Engine/Base/WinApp.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Base/SafeDelete.h"
#include "Externals/imgui/ImGuiManager.h"
#include "Engine/OBJ/ModelManager.h"
#include "Engine/OBJ/OBJ.h"
#include "Engine/Sprite/Sprite.h"
#include "Engine/Resource/Texture/TextureManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/Collider/CollisionManager.h"
#include "Engine/GlobalVariables/GlobalVariables.h"
#include "Engine/Input/Input.h"
#include "Engine/Audio/Audio.h"

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	// 空のインスタンスを生成
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon = nullptr;
	GameObjectManager* gameObjectManager = nullptr;
	CollisionManager* collisionManager = nullptr;
	SceneManager* sceneManager = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;

	// ウィンドウズアプリケーションクラスのインスタンスを取得
	winApp = WinApp::GetInstance();
	// ゲームウィンドウを生成する
	winApp->CreateGameWindow(L"KEngine");

	// DirectX汎用クラスのインスタンスを取得
	dxCommon = DirectXCommon::GetInstance();
	// DirectXの初期化
	dxCommon->Initialize(winApp);

	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetImstance();
	imguiManager->Intialize(winApp, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// モデルマネージャの初期化
	ModelManager::GetInstance()->Initialize();

	// objの静的初期化
	OBJ::StaticInitialize(dxCommon->GetDevice());
	// Spriteの静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), winApp->kWindowWidth, winApp->kwindowHeight);

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Intialize(dxCommon->GetDevice());
	// サンプルテクスチャロード
	TextureManager::Load("white1x1.png"); // white1x1
	TextureManager::Load("./Engine/Resource/Samples/Texture", "ColorSample.png"); // カラーサンプル
	TextureManager::Load("./Engine/Resource/Samples/Texture", "AlphabetSheet.png"); // アルファベットシート

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

	// ゲームオブジェクトマネージャーの初期化
	gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Initialize();

	// 衝突マネージャのインスタンス取得
	collisionManager = CollisionManager::GetInstance();

	// シーンマネージャ初期化
	sceneManager = SceneManager::GetInstance();
	sceneManager->Initialize();

	ImGui::CreateContext();
	auto& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// メインループ
	while (true) {

		// ゲームが終了するメッセージを受けとったら
		if (winApp->ProcessMessage()) {
			// ゲーム終了
			break;
		}
		else {

			// ImGui受付開始
			imguiManager->Begin();

			// 入力関連の毎フレーム処理
			input->Update();

			// 衝突判定リストクリア
			collisionManager->ListClear();

			// グローバル変数の更新
			GlobalVariables::GetInstance()->Update();

			// シーンの更新処理
			sceneManager->Update();
			// ゲームオブジェクト更新
			gameObjectManager->Update();

			// 衝突判定検証
			collisionManager->CheckAllCollision();

			// ImGui受付終了
			imguiManager->End();

			// 描画
			dxCommon->Draw();

			// ImGui描画
			imguiManager->Draw();

			// 描画終了
			dxCommon->PostDraw();
		}

	}

	// 全オブジェクト削除
	gameObjectManager->Initialize();
	// 音解放処理
	audio->Finalize();

	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウを閉じる
	winApp->TerminateGameWindow();

	// COM終了
	CoUninitialize();

	// 終了
	return 0;

}