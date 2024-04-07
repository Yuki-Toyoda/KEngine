#pragma once
#include <vector>
#include "IDescriptorHeap.h"

/// <summary>
/// レンダーターゲットビュークラス
/// </summary>
class RTV : public IDescriptorHeap
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="hwnd">ウィンドウハンドル</param>
	/// <param name="device">使用中のデバイス</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	/// <param name="queue">コマンドキュー</param>
	void Init(HWND hwnd, DirectXDevice* device, int32_t width, int32_t height, ID3D12CommandQueue* queue);

	/// <summary>
	/// レンダーターゲットビューをクリアする関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void ClearRenderTargetView(ID3D12GraphicsCommandList* cmdList);

public: // アクセッサ等

	/// <summary>
	/// RTVの設定ゲッター
	/// </summary>
	/// <returns>RTVの設定</returns>
	D3D12_RENDER_TARGET_VIEW_DESC GetRTVDesc()const { return rtvDesc_; }

	/// <summary>
	/// スワップチューンゲッター
	/// </summary>
	/// <returns>スワップチューン</returns>
	IDXGISwapChain4* GetSwapChain()const { return swapChain_.Get(); }
	/// <summary>
	/// スワップチューン設定ゲッター
	/// </summary>
	/// <returns>スワップチューン設定</returns>
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc()const { return swapChainDesc_; }

	/// <summary>
	/// 書き込み予定バッファのインデックスゲッター
	/// </summary>
	/// <returns>書き込み予定バッファのインデックス</returns>
	UINT GetBackBufferIndex()const { return swapChain_->GetCurrentBackBufferIndex(); }

	/// <summary>
	/// 書き込み予定バッファの本体ゲッター
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetBackBuffer()const { return backBuffers_[swapChain_->GetCurrentBackBufferIndex()].Get(); }

private: // メンバ関数

	// スワップチューン
	Microsoft::WRL::ComPtr<IDXGISwapChain4> swapChain_;
	// スワップチューンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc_;

	// バッファ配列
	std::vector<Microsoft::WRL::ComPtr<ID3D12Resource>> backBuffers_;

	// RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc_;
};

