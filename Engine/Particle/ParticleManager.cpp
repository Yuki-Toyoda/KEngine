#include "ParticleManager.h"
#include "../Base/DirectXCommon.h"

ParticleManager& ParticleManager::Init()
{
	// 各配列要素のクリア
	psos_.clear();
	particles_.clear();

	// ルートシグネチャおよびPSOの生成時に必要物を取得する
	device_  = DirectXCommon::GetInstance()->GetDirectXDevice();							// デバイス
	dxc_	 = DirectXCommon::GetInstance()->GetRendererManager()->GetDXC();				// DXC
	srv_	 = DirectXCommon::GetInstance()->GetSRV();										// SRV
	cmdList_ = DirectXCommon::GetInstance()->GetRendererManager()->GetCommand()->List();	// コマンドリスト

	// シグネチャの初期化
	root_.Init();
	// エミッタデータ用パラメータ生成
	root_.CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// パーティクルデータ用パラメータ生成
	root_.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
	// デバイスをセットする
	root_.SetDevice(device_->GetDevice());
	// ルートシグネチャを生成する
	root_.CreateRootSignature();
}

void ParticleManager::Clear()
{

}

void ParticleManager::Update()
{

}

ParticleManager& ParticleManager::CreateParticlePSO(const std::string& name, const std::string& initShaderPass, const std::string& emitShaderPass, const std::string& updateShaderPass)
{	
	// 初期化時PSOの生成
	 psos_[name].init.Init(root_.GetRootSignature(), dxc_, PSO::PSOType::Compute)
		.SetComputeShader(initShaderPass)
		.Build(device_->GetDevice());

	 // 生成時PSOの生成
	 psos_[name].emit.Init(root_.GetRootSignature(), dxc_, PSO::PSOType::Compute)
		 .SetComputeShader(emitShaderPass)
		 .Build(device_->GetDevice());

	 // 更新時PSOの生成
	 psos_[name].update.Init(root_.GetRootSignature(), dxc_, PSO::PSOType::Compute)
		 .SetComputeShader(updateShaderPass)
		 .Build(device_->GetDevice());
}

Particle* ParticleManager::CreateNewParticle(const std::string& name)
{
	
}

