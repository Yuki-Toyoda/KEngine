#include "Particle.h"
#include "../Model/ModelManager.h"

void Particle::Init(DirectXDevice* device, SRV* srv, ID3D12GraphicsCommandList6* list, const ParticlePSO& pso, const std::string& filePath, const std::string& fileName, const float lifeTime)
{
	// PSOの取得
	pso_ = pso;

	// コマンドリスト取得
	cmdList_ = list;

	// モデル読み込み
	model_ = ModelManager::GetInstance()->CreateParticleModel(filePath, fileName);

	// タイマーを指定された秒数で開始
	timer_.Start(lifeTime);

	/// バッファ生成
	// エミッタ
	emitterDataBuffer_ = std::make_unique<ConstantBuffer<EmitterSphere>>();	// 生成
	emitterDataBuffer_->Init(device);										// 初期化
	// フレーム時間計測
	perFrameDataBuffer_ = std::make_unique<ConstantBuffer<PerFrame>>();	// 生成
	perFrameDataBuffer_->Init(device);									// 初期化
	// 情報
	infoDataBuffer_ = std::make_unique<ConstantBuffer<InfoData>>();	// 生成
	infoDataBuffer_->Init(device);									// 初期化
	// パーティクル
	particleBuffer_ = std::make_unique<RWStructuredBuffer<ParticleData>>(kMaxParticleCount_);	// 生成
	particleBuffer_->Init(device, srv);															// 初期化
	// 使用可能番号
	freeIndexBuffer_ = std::make_unique<RWStructuredBuffer<int32_t>>(1);	// 生成
	freeIndexBuffer_->Init(device, srv);									// 初期化
	// 使用可能番号リスト
	freeIndexListBuffer_ = std::make_unique<RWStructuredBuffer<uint32_t>>(kMaxParticleCount_);	// 生成
	freeIndexListBuffer_->Init(device, srv);													// 初期化

	/// バッファ初期設定
	// フレーム時間計測
	perFrameDataBuffer_->data_->deltaTime = 1.0f / 60.0f;	// 1フレーム秒数
	perFrameDataBuffer_->data_->time = 0.0f;				// 経過秒数リセット
	// 情報
	infoDataBuffer_->data_->instanceCount = kMaxParticleCount_; // パーティクル描画数
	infoDataBuffer_->data_->isBillboard = true;					// ビルボード設定
}

void Particle::ExecuteInit()
{
	// 初期化用PSOをセットする
	cmdList_->SetPipelineState(pso_.init.GetState());

	// パーティクルの初期化に必要なバッファをセットする
	cmdList_->SetComputeRootConstantBufferView(0, emitterDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootConstantBufferView(1, perFrameDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootConstantBufferView(2, infoDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootDescriptorTable(3, particleBuffer_->GetUAVView());
	cmdList_->SetComputeRootDescriptorTable(4, freeIndexBuffer_->GetUAVView());
	cmdList_->SetComputeRootDescriptorTable(5, freeIndexListBuffer_->GetUAVView());

	// パーティクルの初期化を実行する
	cmdList_->Dispatch(UINT(kMaxParticleCount_ + 1023) / 1024, 1, 1);

	// 初期化完了
	isInit_ = true;
}

void Particle::Update()
{
	// 生成用PSOをセットする
	cmdList_->SetPipelineState(pso_.emit.GetState());

	// パーティクルの生成に必要なバッファをセットする
	cmdList_->SetComputeRootConstantBufferView(0, emitterDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootConstantBufferView(1, perFrameDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootConstantBufferView(2, infoDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootDescriptorTable(3, particleBuffer_->GetUAVView());
	cmdList_->SetComputeRootDescriptorTable(4, freeIndexBuffer_->GetUAVView());
	cmdList_->SetComputeRootDescriptorTable(5, freeIndexListBuffer_->GetUAVView());

	// パーティクルの生成を実行する
	cmdList_->Dispatch(1, 1, 1);

	// エミッタの秒数を加算する
	emitterDataBuffer_->data_->frequencyTime += 1.0f / 60.0f;
	// 秒数が射出可能秒数を超えていた場合
	if (emitterDataBuffer_->data_->frequency <= emitterDataBuffer_->data_->frequencyTime) {
		// エミッタの秒数をリセット
		emitterDataBuffer_->data_->frequency = 0.0f;
		// 生成を行う
		emitterDataBuffer_->data_->emit = true;
	}
	else {
		// 秒数が超えていない場合生成を行わない
		emitterDataBuffer_->data_->emit = false;
	}

	// 描画するインスタンス数を指定する
	infoDataBuffer_->data_->instanceCount = kMaxParticleCount_;

	// フレーム時間加算
	perFrameDataBuffer_->data_->time = perFrameDataBuffer_->data_->deltaTime;

	/// この時点で生成処理をしながら更新を行わない用にバリアを張る
	// 　バリア定義
	D3D12_RESOURCE_BARRIER barrier{};
	// バリアの設定を行う
	barrier.Type		  = D3D12_RESOURCE_BARRIER_TYPE_UAV;		  // バリアのタイプ設定
	barrier.Flags		  = D3D12_RESOURCE_BARRIER_FLAG_NONE;		  // フラッグ設定
	barrier.UAV.pResource = particleBuffer_.get()->GetResource(); // バリアを張る対象リソース
	// バリアを張る
	cmdList_->ResourceBarrier(1, &barrier);

	// 更新用PSOをセットする
	cmdList_->SetPipelineState(pso_.update.GetState());

	// パーティクルの更新に必要なバッファをセットする
	cmdList_->SetComputeRootConstantBufferView(0, emitterDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootConstantBufferView(1, perFrameDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootConstantBufferView(2, infoDataBuffer_->GetGPUView());
	cmdList_->SetComputeRootDescriptorTable(3, particleBuffer_->GetUAVView());
	cmdList_->SetComputeRootDescriptorTable(4, freeIndexBuffer_->GetUAVView());
	cmdList_->SetComputeRootDescriptorTable(5, freeIndexListBuffer_->GetUAVView());

	// パーティクルの更新を実行する
	cmdList_->Dispatch(UINT(kMaxParticleCount_ + 1023) / 1024, 1, 1);

	// タイマー更新
	timer_.Update();
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
	cmdList_->SetGraphicsRootDescriptorTable(7, particleBuffer_->GetSRVView());					// パーティクル情報
	cmdList_->SetGraphicsRootDescriptorTable(8, model_->materialsBuffer_->GetGPUView());		// マテリアル

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList_->DispatchMesh(m->GetMeshletCount(), 1, 1);
}
