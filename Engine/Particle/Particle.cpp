#include "Particle.h"

void Particle::Init(DirectXDevice* device, SRV* srv, ID3D12GraphicsCommandList6* list, const ParticlePSO& pso)
{
	// PSOの取得
	pso_ = pso;

	// コマンドリスト取得
	cmdList_ = list;

	/// バッファ生成
	// エミッタ
	emitterDataBuffer_ = std::make_unique<ConstantBuffer<EmitterSphere>>();	// 生成
	emitterDataBuffer_->Init(device);										// 初期化
	// 情報
	infoDataBuffer_ = std::make_unique<ConstantBuffer<InfoData>>();	// 生成
	infoDataBuffer_->Init(device);									// 初期化
	// パーティクル
	particleDataBuffer_ = std::make_unique<RWStructuredBuffer<ParticleData>>(kMaxParticleCount_);	// 生成
	particleDataBuffer_->Init(device, srv);																// 初期化

	// 初期設定
	infoDataBuffer_->data_->instanceCount = kMaxParticleCount_; // パーティクル描画数
	infoDataBuffer_->data_->isBillboard = true;					// ビルボード設定

	// 初期化用PSOをセットする
	cmdList_->SetPipelineState(pso_.init.GetState());

	// パーティクルの初期化に必要なバッファをセットする
	cmdList_->SetComputeRootConstantBufferView(0, emitterDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootDescriptorTable(1, particleDataBuffer_->GetUAVView());

	// パーティクルの初期化を実行する
	cmdList_->Dispatch(UINT(kMaxParticleCount_ + 1023) / 1024, 1, 1);
}

void Particle::Update()
{
	// 生成用PSOをセットする
	cmdList_->SetPipelineState(pso_.emit.GetState());

	// パーティクルの初期化に必要なバッファをセットする
	cmdList_->SetComputeRootConstantBufferView(0, emitterDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootDescriptorTable(1, particleDataBuffer_->GetUAVView());

	// パーティクルの生成を実行する
	cmdList_->Dispatch(UINT(kMaxParticleCount_ + 1023) / 1024, 1, 1);

	// 描画するインスタンス数を指定する
	infoDataBuffer_->data_->instanceCount = kMaxParticleCount_;

	// 更新用PSOをセットする
	cmdList_->SetPipelineState(pso_.update.GetState());

	// パーティクルの初期化を行う
	cmdList_->SetComputeRootConstantBufferView(0, emitterDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootDescriptorTable(1, particleDataBuffer_->GetUAVView());

	// パーティクルの更新を実行する
	cmdList_->Dispatch(UINT(kMaxParticleCount_ + 1023) / 1024, 1, 1);
}

void Particle::Draw()
{
	// モデルデータの取得
	ModelData* m = model_->GetModelData();

	// 描画用バッファをコマンドリストにセットする
	cmdList_->SetGraphicsRootConstantBufferView(2, infoDataBuffer_->GetGPUView());				// パーティクル情報
	cmdList_->SetGraphicsRootDescriptorTable(3, m->meshletBuffer_->GetGPUView());				// メッシュレット情報
	cmdList_->SetGraphicsRootDescriptorTable(4, m->vertexBuffer_->GetGPUView());				// 頂点情報
	cmdList_->SetGraphicsRootDescriptorTable(5, m->uniqueVertexIndicesBuffer_->GetGPUView());	// 固有頂点情報
	cmdList_->SetGraphicsRootDescriptorTable(6, m->primitiveIndicesBuffer_->GetGPUView());		// プリミティブインデックス情報
	cmdList_->SetGraphicsRootDescriptorTable(7, particleDataBuffer_->GetSRVView());				// パーティクル情報
	cmdList_->SetGraphicsRootDescriptorTable(8, model_->materialsBuffer_->GetGPUView());		// マテリアル

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList_->DispatchMesh(m->GetMeshletCount(), 1, 1);
}
