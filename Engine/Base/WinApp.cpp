#include "WinApp.h"
#pragma comment(lib, "winmm.lib")

#include "../../externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT
ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
// ウィンドウ名
const wchar_t WinApp::kWindowClassName[] = L"DirectXGame";

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
/// <returns>シングルトンインスタンス</returns>
WinApp* WinApp::GetInstance() {
	// クラスのインスタンスを取得
	static WinApp instance;
	// インスタンスを返す
	return &instance;
}

/// <summary>
/// ウィンドウプロシージャ
/// </summary>
/// <param name="hwnd">ウィンドウハンドル</param>
/// <param name="msg">メッセージ番号</param>
/// <param name="wparam">メッセージ番号1</param>
/// <param name="lparam">メッセージ番号2</param>
/// <returns>成否</returns>
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	// imGui1でウィンドウの入力を受け取る
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		return true;
	// メッセージに応じてゲーム固有の処理を行う
	switch (msg)
	{
		// ウィンドウが破棄されたら
	case WM_DESTROY:
		// OSに対してアプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);

}

/// <summary>
/// ゲームウィンドウの作成関数
/// </summary>
/// <param name="title">ウィンドウタイトル</param>
/// <param name="windowStyle">ウィンドウの初期スタイル</param>
/// <param name="clientWidth">ウィンドウのクライアント領域の初期横幅</param>
/// <param name="clientHeight">ウィンドウのクライアント領域の初期縦幅</param>
void WinApp::CreateGameWindow(const wchar_t* title, UINT windowStyle,
	int32_t clientWidth, int32_t clientHeight) {

	// ウィンドウクラスの設定
	wndClass_.lpfnWndProc = (WNDPROC)WindowProc;         // ウィンドウプロシージャ
	wndClass_.lpszClassName = kWindowClassName;          // ウィンドウクラス名
	wndClass_.hInstance = GetModuleHandle(nullptr);      // ウィンドウハンドル
	wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);  // カーソル指定

	// 設定したウィンドウクラスをOSに登録する
	RegisterClass(&wndClass_);

	// ウィンドウサイズの設定
	RECT wrc = { 0, 0, clientWidth, clientHeight };
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// 設定を元にウィンドウを作成する
	hwnd_ = CreateWindow(
		wndClass_.lpszClassName, // クラス名
		title,					 // タイトルバー文字
		windowStyle,			 // ウィンドウのスタイル
		CW_USEDEFAULT,			 // ウィンドウを表示するx座標	
		CW_USEDEFAULT,			 // ウィンドウを表示するy座標
		wrc.right - wrc.left,	 // ウィンドウ横幅
		wrc.bottom - wrc.top,    // ウィンドウ縦幅
		nullptr,				 // 親ウィンドウハンドル
		nullptr,				 // メニューハンドル
		wndClass_.hInstance,	 // インスタンスハンドル
		nullptr					 // オプション
	);

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);

	// システムタイマーの分解能を上げる
	timeBeginPeriod(1);

}

/// <summary>
/// ゲームウィンドウの破棄関数
/// </summary>
void WinApp::TerminateGameWindow() {
	// ウィンドウの破棄を行う
	CloseWindow(hwnd_);
}

/// <summary>
/// メッセージの処理
/// </summary>
/// <returns>終了がどうか</returns>
bool WinApp::ProcessMessage() {

	// メッセージを受け取る
	MSG msg{};

	// メッセージがあるのか
	if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	// 終了メッセージが来た場合ループを抜ける
	if (msg.message == WM_QUIT) {
		return true;
	}

	// 終了メッセージが来ていない場合は処理を続ける
	return false;

}
