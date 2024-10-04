#include "SkinModelRootSignature.h"

void SkinModelRootSignature::Init()
{
	// 頂点数用パラメータ生成
	CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// マトリックスパレット用パラメータ生成
	CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// 頂点データ用パラメータ生成
	CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// 出力頂点データ用パラメータ生成
	CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
}
