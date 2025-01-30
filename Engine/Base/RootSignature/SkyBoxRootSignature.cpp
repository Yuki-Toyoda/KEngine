#include "SkyBoxRootSignature.h"

void SkyBoxRootSignature::Init()
{
	// 汎用データ用パラメータ生成
	CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// ワールドトランスフォーム用パラメータ生成
	CreateCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// 頂点データ用パラメータ生成
	CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// インデックスデータ用パラメータ生成
	CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// キューブマップ用パラメータ生成
	CreateDescriptorTableParameter(2, D3D12_SHADER_VISIBILITY_ALL);

	// サンプラー生成
	CreateSampler(0);
}
