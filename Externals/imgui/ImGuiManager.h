#pragma once
#include <d3d12.h>
#include <wrl.h>

#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "../../Engine/Base/DescriptorHeaps/SRV.h"

// クラスの前方宣言
class WinApp;
class DirectXCommon;

/// <summary>
/// ImGui関連のマネージャー
/// </summary>
class ImGuiManager{
public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	static ImGuiManager* GetImstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="win">ウィンドウズアプリケーション</param>
	/// <param name="dxCommon">DirectX12汎用クラス</param>
	void Intialize(WinApp* win, DirectXCommon* dxCommon);

	/// <summary>
	/// 終了関数
	/// </summary>
	void Finalize();

	/// <summary>
	/// ImGui受付開始
	/// </summary>
	void Begin();

	/// <summary>
	/// ImGui受付終了
	/// </summary>
	void End();

private: // コンストラクタ等

	// コンストラクタと代入演算子無効
	ImGuiManager() = default;
	~ImGuiManager() = default;
	ImGuiManager(const ImGuiManager&) = delete;
	const ImGuiManager& operator=(const ImGuiManager&) = delete;

private: // メンバ変数
	// DirectX基盤インスタンス（借りてくる）
	DirectXCommon* dxCommon_ = nullptr;

    // SRV上の登録情報
    SRVInfo srvInfo_;
};

