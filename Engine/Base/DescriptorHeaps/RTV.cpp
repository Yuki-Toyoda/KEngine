#include "RTV.h"

using namespace Microsoft::WRL;

void RTV::Init(HWND hwnd, DirectXDevice* device, int32_t width, int32_t height, ID3D12CommandQueue* queue)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// デバイスの取得
	device_ = device->GetDevice();
	// バッファのサイズを設定
	// 表示用と書き込み用で2枚
	size_ = 2;
	// RTVヒープのバッファ1つ分のサイズを取得
	kDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	// ヒープ生成
	heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, size_, false);

	/// レンダーターゲットビューの設定
	rtvDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // シェーダーで行った計算の結果をSRGBの変換して書き込み
	rtvDesc_.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // 2Dテクスチャ描画として書き込み

	// スワップチューン生成
	// スワップチューンの設定を行う
	swapChainDesc_.Width = width;								   // ウィンドウ横幅
	swapChainDesc_.Height = height;								   // ウィンドウ縦幅
	swapChainDesc_.Format = DXGI_FORMAT_R8G8B8A8_UNORM;			   // 色情報の書式
	swapChainDesc_.SampleDesc.Count = 1;						   // マルチサンプルを行わない
	swapChainDesc_.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;  // 描画のターゲットとして利用する
	swapChainDesc_.BufferCount = 2;								   // バッファ数を二つに設定
	swapChainDesc_.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;	   // モニターに描画し次第破棄する

	// スワップチューン生成時に渡される本体はswapChain1なので変換用
	ComPtr<IDXGISwapChain1> swapChain1;
	// コマンドキューとウィンドウハンドル、スワップチューン設定を渡し、生成
	result = device->GetDXGIFactory()->CreateSwapChainForHwnd(queue, hwnd, &swapChainDesc_, nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result)); // 生成確認

	// 生成出来たらSwapChain4に変換
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));

	// 生成したSwapChainからバッファリソースを取得
	backBuffers_.resize(2); // サイズ設定。今回はダブルバッファなので2
	for (int i = 0; i < backBuffers_.size(); i++) {
		result = swapChain_->GetBuffer(i, IID_PPV_ARGS(&backBuffers_[i])); // 取得
		assert(SUCCEEDED(result)); // 取得確認

		// レンダーターゲットビューを生成
		device_->CreateRenderTargetView(backBuffers_[i].Get(), &rtvDesc_, GetCPUHandle(i));
	}
}

void RTV::ClearRenderTargetView(ID3D12GraphicsCommandList* cmdList)
{
	// クリア色指定
	float clearColor[] = { 0.1f, 0.25f, 0.5f, 1.0f };
	// 書き込み予定バッファを取得し画面全体をクリアする
	cmdList->ClearRenderTargetView(GetCPUHandle(swapChain_->GetCurrentBackBufferIndex()), clearColor, 0, nullptr);
}
