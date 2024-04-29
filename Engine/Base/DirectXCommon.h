#pragma once
#include <Windows.h>
#include <vector>
#include <chrono>
#include <thread>

#include "WinApp.h"
#include "../Debug/Debug.h"
#include "Device/DirectXDevice.h"
#include "DescriptorHeaps/HeapManager.h"

#include "Resource/Rendering/BackBuffer.h"
#include "Resource/Rendering/DepthStencil.h"

#include "../Base/Renderer/RendererManager.h"

// クラスの前方宣言
class Camera;

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
	void Init(WinApp* win);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

public: // アクセッサ等

	/// <summary>
	/// DirectXデバイスゲッター
	/// </summary>
	/// <returns>DirectXデバイス</returns>
	DirectXDevice* GetDirectXDevice() const { return dxDevice_.get(); }

	/// <summary>
	/// デバイスゲッター
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device2* GetDevice() const { return device_; }

	/// <summary>
	/// ヒープマネージャーゲッター
	/// </summary>
	/// <returns>ヒープマネージャー</returns>
	HeapManager* GetHeaps() const { return heaps_.get(); }

	/// <summary>
	/// メインカメラセッター
	/// </summary>
	/// <param name="camera">カメラ</param>
	void SetMainCamera(const Camera* camera);

public: // ImGui用アクセッサ

	/// <summary>
	/// バッファ数ゲッター
	/// </summary>
	/// <returns>バッファ数</returns>
	UINT GetBufferCount() { return swapChainDesc_.BufferCount; }

	/// <summary>
	/// フォーマットゲッター
	/// </summary>
	/// <returns>フォーマット</returns>
	DXGI_FORMAT GetFormat() { return DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; }

	/// <summary>
	/// SRVゲッター
	/// </summary>
	/// <returns>SRVヒープ</returns>
	SRV* GetSRV() { return heaps_->srv(); }

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
	ID3D12Device2* device_ = nullptr;

	// ヒープマネージャー
	std::unique_ptr<HeapManager> heaps_;

	// スワップチューン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// スワップチューン設定用構造体
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;

	// バッファ配列
	std::vector<BackBuffer> backBuffers_;
	// バックバッファDSVリソース
	DepthStencil depthStencil_;

	// 描画マネージャー
	std::unique_ptr<RendererManager> rendererManager_;

};

