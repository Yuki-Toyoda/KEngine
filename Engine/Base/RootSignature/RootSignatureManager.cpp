#include "RootSignatureManager.h"
#include "ModelRootSignature.h"

void RootSignatureManager::Init()
{
	// モデル用のルートシグネチャを生成する
	CreateRootSignature<ModelRootSignature>();
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
