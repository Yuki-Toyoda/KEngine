#pragma once
#include <Windows.h>
#include <cstdint>

/// <summary>
/// ウィンドウズアプリケーションクラス
/// </summary>
class WinApp
{
public: // 静的なメンバ変数

	// ウィンドウサイズ
	static const int kWindowWidth = 1280; // 横
	static const int kwindowHeight = 720; // 縦

	// ウィンドウクラス名
	static const wchar_t kWindowClassName[];

public: // 静的なメンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static WinApp* GetInstance();

	/// <summary>
	/// ウィンドウプロシージャ
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="msg">メッセージ番号</param>
	/// <param name="wparam">メッセージ番号1</param>
	/// <param name="lparam">メッセージ番号2</param>
	/// <returns>成否</returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

public: // メンバ関数

	/// <summary>
	/// ゲームウィンドウの作成関数
	/// </summary>
	/// <param name="title">ウィンドウタイトル</param>
	/// <param name="windowStyle">ウィンドウの初期スタイル</param>
	/// <param name="clientWidth">ウィンドウのクライアント領域の初期横幅</param>
	/// <param name="clientHeight">ウィンドウのクライアント領域の初期縦幅</param>
	void CreateGameWindow(
		const wchar_t* title = L"DirectXGame", UINT windowStyle = WS_OVERLAPPEDWINDOW,
		int32_t clientWidth = kWindowWidth, int32_t clientHeight = kwindowHeight);

	/// <summary>
	/// ゲームウィンドウの破棄関数
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns>終了がどうか</returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns>ウィンドウハンドル</returns>
	HWND GetHwnd() const { return hwnd_; }

	/// <summary>
	/// HINSTANCEの取得
	/// </summary>
	/// <returns>HINSTANCE</returns>
	HINSTANCE GetHInstance() const { return wndClass_.hInstance; }

private: // メンバ関数

	// シングルトンパターンの設定
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	const WinApp& operator=(const WinApp&) = delete;

private: // メンバ変数

	// Window関連
	HWND hwnd_ = nullptr; // ウィンドウハンドル
	WNDCLASS wndClass_{}; // ウィンドウクラス

};

