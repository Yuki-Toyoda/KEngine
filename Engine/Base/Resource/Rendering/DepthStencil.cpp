#include "DepthStencil.h"

void DepthStencil::Init(DirectXDevice* device, HeapManager* heaps)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// Resourceの設定
	desc_.Width			   = UINT(width_);							  // 解像度横幅
	desc_.Height		   = UINT(height_);							  // 解像度縦幅
	desc_.MipLevels		   = 1;										  // ミップマップ数
	desc_.DepthOrArraySize = 1;										  // 奥行き
	desc_.Format		   = DXGI_FORMAT_D24_UNORM_S8_UINT;			  // DepthStencilとして利用可能なフォーマット
	desc_.SampleDesc.Count = 1;										  // サンプリングカウント設定
	desc_.Dimension		   = D3D12_RESOURCE_DIMENSION_TEXTURE2D;	  // テクスチャとして扱う
	desc_.Flags			   = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; // 深度用フラッグを設定

	// 画面クリア設定
	clearColor_.DepthStencil.Depth = 1.0f;							// 深度最大値
	clearColor_.Format			   = DXGI_FORMAT_D24_UNORM_S8_UINT; // フォーマット設定

	// 利用するヒープの設定
	properties_.Type = D3D12_HEAP_TYPE_DEFAULT; // VRAM上に生成

	// バリア設定
	currentBarrierState_ = D3D12_RESOURCE_STATE_DEPTH_WRITE; // 深度値を書き込む設定にする

	// リソースを生成する
	result = device->GetDevice()->CreateCommittedResource(
		&properties_,			 // ヒープ設定
		D3D12_HEAP_FLAG_NONE,	 // フラッグ設定
		&desc_,					 // リソース設定
		currentBarrierState_,	 // バリア設定
		&clearColor_,			 // クリア最適値
		IID_PPV_ARGS(&resource_) // 生成するリソースのポインタ
	);

	// 生成に失敗した場合停止させる
	assert(SUCCEEDED(result));

	// 登録したDSV上のインデックスを取得する
	dsvInfo_ = heaps->dsv()->CreateDepthStencilView(resource_.Get());
}

void DepthStencil::Clear(ID3D12GraphicsCommandList6* list)
{
	// 指定した深度値で画面全体をクリアする
	list->ClearDepthStencilView(dsvInfo_.cpuView_, D3D12_CLEAR_FLAG_DEPTH, clearColor_.DepthStencil.Depth, 0, 0, nullptr);
}
