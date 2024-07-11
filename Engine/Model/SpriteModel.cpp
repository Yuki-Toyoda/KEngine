#include "SpriteModel.h"
#include "../Base/DirectXCommon.h"

void SpriteModel::Init(ModelData* modelData)
{
	// 基底クラスの初期化処理を行う
	IModel::Init(modelData);
	// 渡されたモデルは平面のはずなので最初のマテリアルを取得
	material_ = modelData->materials_[0];
	// 頂点データを取得する
	vertices_ = modelData_->vertices_;

	// トランスフォームの初期化
	transform_.Init();

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	// トランスフォーム用のバッファ生成
	transformBuffer_ = std::make_unique<ConstantBuffer<Matrix4x4>>(); // 生成
	transformBuffer_->Init(device);									  // 初期化
	// マテリアル用のバッファを生成
	materialBuffer_ = std::make_unique<ConstantBuffer<MaterialData>>(); // 生成
	materialBuffer_->Init(device);										// 初期化
	// 頂点データ用のバッファを生成
	vertexBuffer_ = std::make_unique<StructuredBuffer<VertexData>>(static_cast<int32_t>(vertices_.size())); // 生成
	vertexBuffer_->Init(device, srv);																		// 初期化
}

void SpriteModel::Update()
{
	// マテリアルデータ変換保持用変数
	MaterialData material;
	// マテリアルをマテリアルデータに返還
	material = material_;

	// トランスフォームデータのコピー
	*transformBuffer_->data_ = transform_.GetMatWorld();
	// マテリアルデータのコピー
	*materialBuffer_->data_ = material;
	// 頂点データのコピー
	std::memcpy(vertexBuffer_->data_, vertices_.data(), sizeof(VertexData) * vertices_.size());
}

void SpriteModel::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 描画用バッファをコマンドリストにセットする
	cmdList->SetGraphicsRootConstantBufferView(2, transformBuffer_->GetGPUView());					   // トランスフォーム
	cmdList->SetGraphicsRootConstantBufferView(3, materialBuffer_->GetGPUView());					   // マテリアル
	cmdList->SetGraphicsRootDescriptorTable(4, modelData_->meshletBuffer_->GetGPUView());			   // メッシュレット情報
	cmdList->SetGraphicsRootDescriptorTable(5, vertexBuffer_->GetGPUView());						   // 頂点情報
	cmdList->SetGraphicsRootDescriptorTable(6, modelData_->uniqueVertexIndicesBuffer_->GetGPUView());  // 固有頂点情報
	cmdList->SetGraphicsRootDescriptorTable(7, modelData_->primitiveIndicesBuffer_->GetGPUView());	   // プリミティブインデックス情報

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(modelData_->GetMeshletCount(), 1, 1);
}
