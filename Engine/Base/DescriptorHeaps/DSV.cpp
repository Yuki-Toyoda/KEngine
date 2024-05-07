#include "DSV.h"

DSV::DSV(ID3D12Device2* device) :
	IDescriptorHeap(device, D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1){}

void DSV::Init()
{
	// ヒープ生成
	heap_ = CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, kSize_, false);
}

DSVInfo DSV::CreateDepthStencilView(ID3D12Resource* resource)
{
	// 返還用
	DSVInfo info{};

	// 各種設定を行う
	info.desc.Format	    = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット設定
	info.desc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; // テクスチャとして扱う

	// 空いているインデックスを取得する
	info.index_ = indexList_.UseEmpty();
	// アドレスの取得
	info.SetView(this);

	// DSVの生成を行う
	device_->CreateDepthStencilView(resource, &info.desc, info.cpuView_);

	// 生成後、情報構造体を返す
	return info;
}
