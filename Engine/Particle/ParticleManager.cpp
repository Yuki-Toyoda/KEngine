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
	// パーティクル用カメラデータ用パラメータ生成
	root_.CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	// エミッタデータ用パラメータ生成
	root_.CreateCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	// フレーム時間計測用パラメータ生成
	root_.CreateCBVParameter(2, D3D12_SHADER_VISIBILITY_ALL);
	// 情報用パラメータ生成
	root_.CreateCBVParameter(3, D3D12_SHADER_VISIBILITY_ALL);
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

	// フレーム時間計測用バッファの生成
	perFrameDataBuffer_ = std::make_unique<ConstantBuffer<PerFrame>>();	// 生成
	perFrameDataBuffer_->Init(device_);									// 初期化
	// フレーム時間計測
	perFrameDataBuffer_->data_->timeScale = 1.0f;			// ゲーム速度
	perFrameDataBuffer_->data_->deltaTime = 1.0f / 60.0f;	// 1フレーム秒数
	perFrameDataBuffer_->data_->time = 0.0f;				// 経過秒数リセット

	// パーティクルを更新する状態に戻す
	isUpdateAllParticles_ = true;

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
	
	// 生存時間を超過したパーティクルを削除
	particles_.remove_if([](std::unique_ptr<Particle>& p) {
		if (p->GetIsEnd()) {
			// パーティクルに使用しているモデルを削除
			p->model_->isDestroy_ = true;

			return true;
		}
		return false;
	});

	// ルートシグネチャをセットする
	cmdList_->SetComputeRootSignature(root_.GetRootSignature());

	// 全パーティクル更新
	for (std::unique_ptr<Particle>& p : particles_) {
		// 初期化処理
		if (!p->GetIsInit()) {
			// フレーム時間計測用バッファをコマンドリストにセットする
			cmdList_->SetComputeRootConstantBufferView(2, perFrameDataBuffer_->GetGPUView());
			// 初期化実行
			p->ExecuteInit();
		}

		// 更新を行わない状態の場合、初期化を行った後次へ
		if (!isUpdateAllParticles_) { continue; }

		// フレーム時間計測用バッファをコマンドリストにセットする
		cmdList_->SetComputeRootConstantBufferView(2, perFrameDataBuffer_->GetGPUView());
		// 更新処理
		p->Update(perFrameDataBuffer_->data_->timeScale);
	}

	// フレーム時間加算
	perFrameDataBuffer_->data_->time += perFrameDataBuffer_->data_->deltaTime;
}

void ParticleManager::Draw()
{
	// 全パーティクル描画
	for (std::unique_ptr<Particle>& p : particles_) {
		cmdList_->SetComputeRootConstantBufferView(1, perFrameDataBuffer_->GetGPUView());
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

Particle* ParticleManager::CreateNewParticle(const std::string& name, const std::string& filePath, const std::string& fileName, const float lifeTime, const bool isEndless, const int maxCount, const bool enableLighting)
{
	// ルートシグネチャをセットする
	cmdList_->SetComputeRootSignature(root_.GetRootSignature());

	// 新規パーティクル生成
	std::unique_ptr<Particle> newParticle = std::make_unique<Particle>(maxCount);
	// パーティクル初期化
	newParticle->Init(device_, srv_, cmdList_, psos_[name], filePath, fileName, lifeTime, isEndless, enableLighting);

	// インスタンス返還用のモデルを取得
	Particle* returnParticle = newParticle.get();

	// 生成したモデルをリストに追加
	particles_.push_back(std::move(newParticle));

	// 生成したモデルを返す
	return returnParticle;
}
