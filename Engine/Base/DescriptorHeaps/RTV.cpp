#include "RTV.h"

RTV::RTV(ID3D12Device2* device) :
	IDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_RTV, KEngine::Config::Rendering::kMaxMultiRendering + 2){}

void RTV::Init(){
	// ヒープ生成
	heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, kSize_, false);
}

RTVInfo RTV::CreateRenderTargetView(ID3D12Resource* resource)
{
	// 返還用
	RTVInfo info;

	// 各種設定を行う
	info.desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;		 // フォーマット設定
	info.desc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D; // テクスチャとして扱う

	// 空のインデックスを取得する
	info.index_ = indexList_.UseEmpty();
	// Viewを設定
	info.SetView(this);

	// RTVの生成を行う
	device_->CreateRenderTargetView(resource, &info.desc, info.cpuView_);

	// 生成後、情報構造体を返す
	return info;
}
