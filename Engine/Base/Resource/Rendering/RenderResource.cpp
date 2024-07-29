#include "RenderResource.h"

void RenderResource::Init(DirectXDevice* device, HeapManager* heaps)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// リソースの設定を行う
	desc_.Width			   = UINT(width_);
	desc_.Height		   = UINT(height_);
	desc_.MipLevels		   = 0;
	desc_.DepthOrArraySize = 1;
	desc_.Format		   = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	desc_.SampleDesc.Count = 1;
	desc_.Dimension		   = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	desc_.Flags			   = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	// 画面クリア色指定
	clearColor_.Format   = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	clearColor_.Color[0] = 0.0f;
	clearColor_.Color[1] = 0.0f;
	clearColor_.Color[2] = 0.0f;
	clearColor_.Color[3] = 1.0f;

	// 利用するヒープの設定を行う
	properties_.Type				 = D3D12_HEAP_TYPE_CUSTOM;			   // 細かい設定を行う
	properties_.CPUPageProperty		 = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK; // WriteBackポリシーでCPUアクセス可能
	properties_.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;			   // プロセッサの近くに配置

	// バリアを設定する
	currentBarrierState_ = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	
	// Resource生成
	result = device->GetDevice()->CreateCommittedResource(
		&properties_,			 // ヒープ設定
		D3D12_HEAP_FLAG_NONE,	 // ヒープの特殊な設定
		&desc_,					 // リソースの設定
		currentBarrierState_,    // 初回のリソースステート
		&clearColor_,			 // クリア最適値
		IID_PPV_ARGS(&resource_) // 作成するResourceへのポインタ
	);
	// 生成成否確認
	assert(SUCCEEDED(result));

	// RTVに登録
	rtvInfo_ = heaps->rtv()->CreateRenderTargetView(resource_.Get());
	// SRVに登録
	srvInfo_ = heaps->srv()->RegisterRenderResource(resource_.Get(), width_, height_);
}

void RenderResource::Clear(ID3D12GraphicsCommandList6* list)
{
	// RTVをクリア
	list->ClearRenderTargetView(rtvInfo_.cpuView_, clearColor_.Color, 0, nullptr);
}
