#pragma once
#include <Windows.h>
#include <vector>
#include <chrono>

#include "WinApp.h"
#include "../Debug/Debug.h"
#include "Device/DirectXDevice.h"
#include "DescriptorHeaps/RTV.h"
#include "DescriptorHeaps/SRV.h"
#include "DescriptorHeaps/DSV.h"
#include "Command/CommandManager.h"

/// <summary>
/// DirectX汎用クラス
/// </summary>
class DirectXCommon
{
public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static DirectXCommon* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="win">ウィンドウズアプリケーションクラス</param>
	/// <param name="backBufferWidth">ウィンドウ横幅</param>
	/// <param name="backBufferHeight">ウィンドウ縦幅</param>
	void Init(WinApp* win, 
		int32_t backBufferWidth = WinApp::kWindowWidth, int32_t backBufferHeight = WinApp::kwindowHeight);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

public: // アクセッサ等

	/// <summary>
	/// デバイスゲッター
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device_; }

	/// <summary>
	/// 描画コマンドマネージャーゲッター
	/// </summary>
	/// <returns>描画コマンドマネージャー</returns>
	CommandManager* GetCommandManager() { return commandManager_.get(); }

	/// <summary>
	/// (ImGui用)バッファ数ゲッター
	/// </summary>
	/// <returns>バッファ数</returns>
	UINT GetBufferCount() { return rtv_->GetSwapChainDesc().BufferCount; }
	/// <summary>
	/// (ImGui用)フォーマット形式ゲッター
	/// </summary>
	/// <returns>フォーマット形式</returns>
	DXGI_FORMAT GetFormat() { return rtv_->GetRTVDesc().Format; }
	/// <summary>
	/// (ImGui用)SRVヒープゲッター
	/// </summary>
	/// <returns>SRVヒープ</returns>
	ID3D12DescriptorHeap* GetSRVHeap() { return srv_->GetDescriptorHeap(); }

private: // メンバ関数

	// シングルトンパターンの設定
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	// FPS固定に用いる記録時間
	std::chrono::steady_clock::time_point reference_;

	/// <summary>
	/// FPS固定の初期化
	/// </summary>
	void InitializeFixFPS();
	/// <summary>
	/// FPS固定の更新
	/// </summary>
	void UpdateFixFPS();

private: // メンバ変数

	// ウィンドウズアプリケーションクラス
	WinApp* winApp_;

	// デバイス
	std::unique_ptr<DirectXDevice> dxDevice_;
	ID3D12Device* device_ = nullptr;

	// 各種ヒープ
	std::unique_ptr<RTV> rtv_; // レンダーターゲットビュー
	std::unique_ptr<SRV> srv_; // シェーダーリソースビュー
	std::unique_ptr<DSV> dsv_; // 深度ステンシルビュー

	// 描画コマンドマネージャー
	std::unique_ptr<CommandManager> commandManager_;

	// ウィンドウ幅
	int32_t backBufferWidth_ = 0;  // 横
	int32_t backBufferHeight_ = 0; // 縦

};

