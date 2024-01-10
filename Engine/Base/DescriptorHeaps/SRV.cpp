#include "SRV.h"

using namespace Microsoft::WRL;

void SRV::Init(ID3D12Device* device)
{
	// デバイスを取得
	device_ = device;
	// サイズ設定
	size_ = 128;
	// SRVヒープ1つ分のサイズを取得
	kDescriptorSize_ = device_->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	// ヒープ生成
	heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV, size_, true);
}
