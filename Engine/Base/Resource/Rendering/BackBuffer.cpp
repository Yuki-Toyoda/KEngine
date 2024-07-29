#include "BackBuffer.h"

BackBuffer::BackBuffer(IDXGISwapChain4* swapchain, int index)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// スワップチューンからバッファを取得
	result = swapchain->GetBuffer(index, IID_PPV_ARGS(&resource_));
	// 取得に失敗していた場合停止させる
	assert(SUCCEEDED(result));
}

void BackBuffer::Init(DirectXDevice* device, HeapManager* heaps)
{
	// 画面クリア色指定
	clearColor_.Format   = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearColor_.Color[0] = 0.0f;
	clearColor_.Color[1] = 0.0f;
	clearColor_.Color[2] = 0.0f;
	clearColor_.Color[3] = 1.0f;

	// バリアの初期値をセット
	currentBarrierState_ = D3D12_RESOURCE_STATE_PRESENT;

	// 警告回避のために記述
	device;

	// RTVに登録する
	rtvInfo_ = heaps->rtv()->CreateRenderTargetView(resource_.Get());
}

void BackBuffer::Clear(ID3D12GraphicsCommandList6* list)
{
	// 画面クリアを行う
	list->ClearRenderTargetView(rtvInfo_.cpuView_, clearColor_.Color, 0, nullptr);
}
