#include "PostProcessRootSignature.h"

void PostProcessRootSignature::Init()
{
	// テクスチャサイズ用定数バッファ
	CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);
	// レンダリング後のテクスチャ
	CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);
	// 深度マップ
	CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_PIXEL);
	// サンプラー生成
	CreateSampler(0);
}
