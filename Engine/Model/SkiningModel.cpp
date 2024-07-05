#include "SkiningModel.h"
#include "../Base/DirectXCommon.h"

void SkiningModel::Init(ModelData* modelData)
{
	// 基底クラスの初期化処理を行う
	IModel::Init(modelData);
	// マテリアルの取得
	materials_ = modelData->materials_;
	// アニメーションの取得
	animationManager_ = modelData->animationManager_;
	// スケルトンの取得
	skelton_ = modelData->skelton_.value();
	// スキンクラスターの参照を保持
	skinCluster_ = &modelData->skinCluster_.value();

	// トランスフォームの初期化
	transform_.Init();

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	// トランスフォーム用のバッファ生成
	transformBuffer_ = std::make_unique<ConstantBuffer<Matrix4x4>>(); // 生成
	transformBuffer_->Init(device);									  // 初期化
	// マテリアル用のバッファを生成
	materialsBuffer_ = std::make_unique<StructuredBuffer<MaterialData>>(static_cast<int32_t>(materials_.size())); // 生成
	materialsBuffer_->Init(device, srv);																		  // 初期化
	// MatrixPaletteの生成
	palletteBuffer_ = std::make_unique<StructuredBuffer<WellForGPU>>(static_cast<uint32_t>(skelton_.joints_.size())); // 生成
	palletteBuffer_->Init(device, srv);																				  // 初期化
}

void SkiningModel::Update()
{
	// マテリアルデータ変換保持用配列
	std::vector<MaterialData> materials;
	// 全マテリアルをマテリアルデータ構造体の変換
	for (int i = 0; i < materials_.size(); i++) {
		materials.push_back(materials_[i]);
	}

	// アニメーションマネージャーの更新
	animationManager_.Update(&skelton_);

	// スケルトン更新関数
	skelton_.Update();

	// 全ジョイント数分ループ
	for (size_t jointIndex = 0; jointIndex < skelton_.joints_.size(); jointIndex++) {
		// インデックスが行列数を超過していた場合停止させる
		assert(jointIndex < skinCluster_->inverseBindPoseMatrices_.size());
		palletteBuffer_->data_[jointIndex].skeltonSpaceMatrix =
			skinCluster_->inverseBindPoseMatrices_[jointIndex] * skelton_.joints_[jointIndex].skeltonSpaceMatrix_;
		palletteBuffer_->data_[jointIndex].skeltonSpaceTransposeMatrix =
			Matrix4x4::MakeTranspose(Matrix4x4::MakeInverse(palletteBuffer_->data_[jointIndex].skeltonSpaceMatrix));
	}

	// トランスフォームデータのコピー
	*transformBuffer_->data_ = transform_.GetMatWorld();
	// マテリアルデータのコピー
	std::memcpy(materialsBuffer_->data_, materials.data(), sizeof(MaterialData) * materials.size());
}

void SkiningModel::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 描画用バッファをコマンドリストにセットする
	cmdList->SetGraphicsRootConstantBufferView(2, transformBuffer_->GetGPUView());					   // トランスフォーム
	cmdList->SetGraphicsRootDescriptorTable(3, modelData_->meshletBuffer_->GetGPUView());			   // メッシュレット情報
	cmdList->SetGraphicsRootDescriptorTable(4, modelData_->vertexBuffer_->GetGPUView());			   // 頂点情報
	cmdList->SetGraphicsRootDescriptorTable(5, modelData_->uniqueVertexIndicesBuffer_->GetGPUView());  // 固有頂点情報
	cmdList->SetGraphicsRootDescriptorTable(6, modelData_->primitiveIndicesBuffer_->GetGPUView());	   // プリミティブインデックス情報
	cmdList->SetGraphicsRootDescriptorTable(7, materialsBuffer_->GetGPUView());						   // マテリアル
	cmdList->SetGraphicsRootDescriptorTable(8, palletteBuffer_->GetGPUView());						   // マトリックスパレット

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(modelData_->GetMeshletCount(), 1, 1);
}