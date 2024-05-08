#include "ModelRootSignature.h"

void ModelRootSignature::Init()
{
	// 汎用データ用パラメータ生成
	CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// ワールドトランスフォーム用パラメータ生成
	CreateCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// メッシュレットデータ用パラメータ生成
	CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// 頂点データ用パラメータ生成
	CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// 固有頂点用パラメータ生成
	CreateDescriptorTableParameter(2, D3D12_SHADER_VISIBILITY_ALL);
	// プリミティブ頂点用パラメータ生成
	CreateDescriptorTableParameter(3, D3D12_SHADER_VISIBILITY_ALL);
}
