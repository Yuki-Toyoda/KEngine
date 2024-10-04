#include "ParticleManager.h"
#include "Engine/Base/DirectXCommon.h"

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
	// フレーム時間計測用パラメータ生成
	root_.CreateCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// 情報用パラメータ生成
	root_.CreateCBVParameter(2, D3D12_SHADER_VISIBILITY_ALL);
	// パーティクルデータ用パラメータ生成
	root_.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
	// 使用可能番号用パラメータ生成
	root_.CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
	// 使用可能番号リスト用パラメータ生成
	root_.CreateDescriptorTableParameter(2, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_UAV);
	
	// デバイスをセットする
	root_.SetDevice(device_->GetDevice());
	// ルートシグネチャを生成する
	root_.CreateRootSignature();

	// 自身を返す
	return *this;
}

void ParticleManager::Clear()
{
	// 全パーティクルモデル削除
	for (std::unique_ptr<Particle>& p : particles_) {
		// パーティクルに使用しているモデルを削除
		p->model_->isDestroy_ = true;
	}

	// 生成済みの全パーティクルの削除を行う
	particles_.clear();
}

void ParticleManager::Update()
{
	// ルートシグネチャをセットする
	cmdList_->SetComputeRootSignature(root_.GetRootSignature());

	// 生存時間を超過したパーティクルを削除
	particles_.remove_if([](std::unique_ptr<Particle>& p) {
		if (p->timer_.GetIsFinish()) {
			// パーティクルに使用しているモデルを削除
			p->model_->isDestroy_ = true;

			return true;
		}
		return false;
	});

	// 全パーティクル更新
	for (std::unique_ptr<Particle>& p : particles_) {
		// 初期化処理
		if (!p->GetIsInit()) {
			p->ExecuteInit();
		}

		// 更新処理
		p->Update();
	}
}

void ParticleManager::Draw()
{
	// 全パーティクル描画
	for (std::unique_ptr<Particle>& p : particles_) {
		p->Draw();
	}
}

ParticleManager& ParticleManager::CreateParticlePSO(const std::string& name, const std::string& initShaderPass, const std::string& emitShaderPass, const std::string& updateShaderPass)
{	
	// 初期化時PSOの生成
	 psos_[name].init.Init(root_.GetRootSignature(), dxc_, PSO::PSOType::Compute)
		.SetComputeShader(defaultDirectory_ + initShaderPass)
		.Build(device_->GetDevice());

	 // 生成時PSOの生成
	 psos_[name].emit.Init(root_.GetRootSignature(), dxc_, PSO::PSOType::Compute)
		 .SetComputeShader(defaultDirectory_ + emitShaderPass)
		 .Build(device_->GetDevice());

	 // 更新時PSOの生成
	 psos_[name].update.Init(root_.GetRootSignature(), dxc_, PSO::PSOType::Compute)
		 .SetComputeShader(defaultDirectory_ + updateShaderPass)
		 .Build(device_->GetDevice());

	 // 自身を返す
	 return *this;
}

Particle* ParticleManager::CreateNewParticle(const std::string& name, const std::string& filePath, const std::string& fileName, const float lifeTime, const bool enableLighting, const int maxCount)
{
	// ルートシグネチャをセットする
	cmdList_->SetComputeRootSignature(root_.GetRootSignature());

	// 新規パーティクル生成
	std::unique_ptr<Particle> newParticle = std::make_unique<Particle>(maxCount);
	// パーティクル初期化
	newParticle->Init(device_, srv_, cmdList_, psos_[name], filePath, fileName, lifeTime, enableLighting);

	// インスタンス返還用のモデルを取得
	Particle* returnParticle = newParticle.get();

	// 生成したモデルをリストに追加
	particles_.push_back(std::move(newParticle));

	// 生成したモデルを返す
	return returnParticle;
}
