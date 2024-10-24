#include "RootSignatureManager.h"
#include "ModelRootSignature.h"
#include "SkinModelRootSignature.h"
#include "SpriteRootSignature.h"
#include "ParticleRootSignature.h"
#include "SkyBoxRootSignature.h"

void RootSignatureManager::Init()
{
	// 各ルートシグネチャの生成
	CreateRootSignature<ModelRootSignature>();		   // モデル用
	CreateRootSignature<SkinModelRootSignature>();     // スキニング計算用
	CreateRootSignature<SpriteRootSignature>();		   // スプライト用
	CreateRootSignature<ParticleRootSignature>();	   // パーティクル用
	CreateRootSignature<SkyBoxRootSignature>();		   // スカイボックス用
}

ID3D12RootSignature* RootSignatureManager::GetRootSignature(const int32_t& index)
{
	// ルートシグネチャの配列範囲外だった場合、停止させる
	if (rootSignatures_.size() < (index + 1)) {
		// ログに出力
		Debug::Log("ルートシグネチャ配列の範囲外です");
		// 停止させる
		assert(false);
	}

	// 指定された引数のRootSignature返す
	return rootSignatures_[index].get()->GetRootSignature();
}
