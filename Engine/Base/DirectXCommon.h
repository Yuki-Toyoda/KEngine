#pragma once
#include <Windows.h>
#include <cstdlib>
#include <wrl.h>
#include <vector>
#include <chrono>

#include <d3d12.h>
#include <dxgi1_6.h>

#include "WinApp.h"
#include "../Debug/Debug.h"

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
	void Initialize(WinApp* win, 
		int32_t backBufferWidth = WinApp::kWindowWidth, int32_t backBufferHeight = WinApp::kwindowHeight);

	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();

	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

	/// <summary>
	/// レンダーターゲットのクリアを行う関数
	/// </summary>
	void ClearRenderTarget();

	/// <summary>
	/// 深度バッファをクリアする関数
	/// </summary>
	void ClearDepthBuffer();

	/// <summary>
	/// デバイスの取得関数
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// スワップチューンのゲッター
	/// </summary>
	/// <returns>スワップチューン</returns>
	IDXGISwapChain4* GetSwapChain() { return swapChain_.Get(); }

	/// <summary>
	/// コマンドリストのゲッター
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList* GetCommandList() { return commandList_.Get(); }

	/// <summary>
	/// バックバッファの数のゲッター
	/// </summary>
	/// <returns>バックバッファ数</returns>
	size_t GetBackBufferCount() { return backBuffers_.size(); }

	/// <summary>
	/// ビューポートのゲッター
	/// </summary>
	/// <returns>ビューポート</returns>
	D3D12_VIEWPORT* GetViewPort() { return &viewport; }

	/// <summary>
	/// シザー矩形のゲッター
	/// </summary>
	/// <returns>シザー矩形</returns>
	D3D12_RECT* GetScissorRect() { return &scissorRect; }

private: // メンバ変数

	// ウィンドウズアプリケーションクラス
	WinApp* winApp_;

	// Direct3D関連
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;				  // DXGIファクトリー
	Microsoft::WRL::ComPtr<ID3D12Device> device_;					  // デバイス
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList_;	  // コマンドリスト
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandAllocator_; // コマンドアロケーター
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> commandQueue_;		  // コマンドキュー
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;				  // スワップチューン
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_; // バックバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> depthBuffer_;			  // 深度バッファ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> rtvHeap_;			  // RTVヒープ
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> dsvHeap_;			  // DSVヒープ
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;						  // フェンス
	UINT64 fenceVal_ = 0;											  // フェンスの値
	int32_t backBufferWidth_ = 0;									  // ウィンドウ横幅
	int32_t backBufferHeight_ = 0;									  // ウィンドウ縦幅

	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};

private: // メンバ関数

	// シングルトンパターンの設定
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	/// <summary>
	/// 特定のインデックスのディスクリプタハンドルを取得する関数(CPU)
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタサイズ</param>
	/// <param name="index">取得するヒープのインデックス</param>
	/// <returns>特定のインデックスのディスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t commitIndex);

	/// <summary>
	/// 特定のインデックスのディスクリプタハンドルを取得する関数(GPU)
	/// </summary>
	/// <param name="descriptorHeap">ディスクリプタヒープ</param>
	/// <param name="descriptorSize">ディスクリプタサイズ</param>
	/// <param name="index">取得するヒープのインデックス</param>
	/// <returns>特定のインデックスのディスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ID3D12DescriptorHeap* descriptorHeap, uint32_t descriptorSize, uint32_t commitIndex);

	/// <summary>
	/// リソースバリアの切り替えを行う関数
	/// </summary>
	/// <param name="resource">リソース</param>
	/// <param name="before">以前のステート</param>
	/// <param name="after">以降のステート</param>
	/// <returns>切り替えたリソースバリア</returns>
	D3D12_RESOURCE_BARRIER SwitchResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

	/// <summary>
	/// DXGIデバイスの初期化
	/// </summary>
	void InitializeDXGIDevice();

	/// <summary>
	/// スワップチューンの作成関数
	/// </summary>
	void CreateSwapChain();

	/// <summary>
	/// コマンド関連の初期化関数
	/// </summary>
	void InitializeCommand();

	/// <summary>
	/// レンダーターゲット生成関数
	/// </summary>
	void CreateFinalRenderTargets();

	/// <summary>
	/// 深度バッファ生成関数
	/// </summary>
	void CreateDepthBuffer();

	/// <summary>
	/// フェンス生成関数
	/// </summary>
	void CreateFence();

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

};

