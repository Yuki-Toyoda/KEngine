#include <Windows.h>
#include "Engine/Base/WinApp.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/Base/SafeDelete.h"
#include "Externals/imgui/ImGuiManager.h"
#include "Engine/Primitive/PrimitiveManager.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/GameObject/GameObjectManager.h"
#include "Engine/Resource/Texture/TextureManager.h"
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
	Input* input = nullptr;
	Audio* audio = nullptr;

	// ウィンドウズアプリケーションクラスのインスタンスを取得
	winApp = WinApp::GetInstance();
	// ゲームウィンドウを生成する
	winApp->CreateGameWindow(L"KEngine");

	// DirectX汎用クラスのインスタンスを取得
	dxCommon = DirectXCommon::GetInstance();
	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Initialize();
	// DirectXの初期化
	dxCommon->Initialize(winApp);

	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetImstance();
	imguiManager->Intialize(winApp, dxCommon);

	// 形状マネージャの初期化
	PrimitiveManager* primitiveManager = PrimitiveManager::GetInstance();
	primitiveManager->Initialize();

	// オブジェクトマネージャーの初期化
	GameObjectManager* gameObjectManager = GameObjectManager::GetInstance();
	gameObjectManager->Initialize();

	// シーンマネージャーの初期化
	SceneManager* sceneManager = SceneManager::GetInstance();
	sceneManager->Initialize();

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// グローバル変数の読み込み
	GlobalVariables::GetInstance()->LoadFiles();

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

			// グローバル変数の更新
			GlobalVariables::GetInstance()->Update();

			// シーンマネージャー更新
			sceneManager->Update();

			// オブジェクトマネージャー更新
			gameObjectManager->Update();

			// DirectX汎用クラスのImGuiを描画
			#ifdef _DEBUG
			dxCommon->GetCommandManager()->DisplayImGui();
			#endif // _DEBUG

			// ImGui受付終了
			imguiManager->End();

			// 形状マネージャ描画
			primitiveManager->Draw();

			// 描画
			dxCommon->Draw();

			// ImGui描画
			imguiManager->Draw();

			// 描画終了
			dxCommon->PostDraw();
		}

	}

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