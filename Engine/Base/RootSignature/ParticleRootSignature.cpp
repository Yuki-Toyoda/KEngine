#include "ParticleRootSignature.h"

void ParticleRootSignature::Init()
{
	// 汎用データ用パラメータ生成
	CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// 平行光源データ用パラメータ生成
	CreateCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// インスタンス数カウント用パラメータ生成
	CreateCBVParameter(2, D3D12_SHADER_VISIBILITY_ALL);
	// メッシュレットデータ用パラメータ生成
	CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// 頂点データ用パラメータ生成
	CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// 固有頂点用パラメータ生成
	CreateDescriptorTableParameter(2, D3D12_SHADER_VISIBILITY_ALL);
	// プリミティブ頂点用パラメータ生成
	CreateDescriptorTableParameter(3, D3D12_SHADER_VISIBILITY_ALL);
	// パーティクルデータ用パラメータ生成
	CreateDescriptorTableParameter(4, D3D12_SHADER_VISIBILITY_ALL);
	// マテリアル用パラメータ生成
	CreateDescriptorTableParameter(5, D3D12_SHADER_VISIBILITY_ALL);
	// テクスチャデータ用パラメータ生成
	CreateDescriptorTableParameter(6, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, KEngine::Config::Rendering::kMaxTexture);

	// サンプラー生成
	CreateSampler(0);
}
