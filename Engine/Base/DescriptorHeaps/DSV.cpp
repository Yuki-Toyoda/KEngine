#include "DSV.h"

void DSV::Init(ID3D12Device* device, int32_t width, int32_t height)
{
	// デバイス取得
	device_ = device;
	// バッファのサイズを設定
	// 表示用と書き込み用で2枚
	size_ = 2;
	// DSVヒープのバッファ1つ分のサイズを取得
	kDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);

	// ヒープ生成
	heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, size_, false);

	// 深度ステンシルビューリソース生成
	CreateDepthStencilResource(width, height);
}

void DSV::ClearDepth(UINT index, ID3D12GraphicsCommandList* cmdList)
{
	// 指定された深度で画面全体をクリア
	cmdList->ClearDepthStencilView(GetCPUHandle(index), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void DSV::CreateDepthStencilResource(int32_t width, int32_t height)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// ヒーププロパティの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	// リソースの設定
	D3D12_RESOURCE_DESC depthResourceDesc{};

	depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	depthResourceDesc.Width = width; // リソースのサイズ　
	depthResourceDesc.Height = height; // リソースのサイズ　
	depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	depthResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	depthResourceDesc.DepthOrArraySize = 1;
	depthResourceDesc.MipLevels = 1;
	depthResourceDesc.SampleDesc.Count = 1;

	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	// 深度バッファリソース生成
	result = device_->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE,
		&depthResourceDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue,
		IID_PPV_ARGS(&depthStencilResource_));
	// 生成出来たか確認
	assert(SUCCEEDED(result));

	// 深度ビュー作成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; // 深度値フォーマット
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	device_->CreateDepthStencilView(
		depthStencilResource_.Get(), &dsvDesc, heap_.Get()->GetCPUDescriptorHandleForHeapStart());
}
