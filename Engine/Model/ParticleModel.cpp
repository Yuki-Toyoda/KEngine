#include "ParticleModel.h"
#include "../Base/DirectXCommon.h"

void ParticleModel::Init(ModelData* modelData)
{
	// 基底クラスの初期化処理を行う
	IModel::Init(modelData);
	// 渡されたモデルは１つのマテリアルに最初のマテリアルを取得
	material_ = modelData->materials_[0];

	// トランスフォームの初期化
	transform_.Init();

	// 配列クリア
	transforms_.clear();
	materials_.clear();
}

void ParticleModel::Update()
{
	// トランスフォームデータのコピー
	std::memcpy(transformsBuffer_->data_, transforms_.data(), sizeof(VertexData) * transforms_.size());
	// マテリアルデータのコピー
	std::memcpy(materialsBuffer_->data_, materials_.data(), sizeof(MaterialData) * materials_.size());

	// トランスフォーム数でモデルの描画数を決める
	*instanceCountBuffer_->data_ = static_cast<int32_t>(transforms_.size());
}

void ParticleModel::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 描画用バッファをコマンドリストにセットする
	cmdList->SetGraphicsRootConstantBufferView(2, instanceCountBuffer_->GetGPUView());				  // 描画数
	cmdList->SetGraphicsRootDescriptorTable(3, modelData_->meshletBuffer_->GetGPUView());			  // メッシュレット情報
	cmdList->SetGraphicsRootDescriptorTable(4, modelData_->vertexBuffer_->GetGPUView());			  // 頂点情報
	cmdList->SetGraphicsRootDescriptorTable(5, modelData_->uniqueVertexIndicesBuffer_->GetGPUView()); // 固有頂点情報
	cmdList->SetGraphicsRootDescriptorTable(6, modelData_->primitiveIndicesBuffer_->GetGPUView());	  // プリミティブインデックス情報
	cmdList->SetGraphicsRootDescriptorTable(7, transformsBuffer_->GetGPUView());					  // トランスフォーム
	cmdList->SetGraphicsRootDescriptorTable(8, materialsBuffer_->GetGPUView());						  // マテリアル

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(modelData_->GetMeshletCount(), 1, 1);

	// 描画後、一時格納配列をクリアする
	transforms_.clear();
	materials_.clear();
}

void ParticleModel::CreateBuffer(int32_t maxDrawCount)
{
	// 最大描画数設定
	maxDrawCount_ = maxDrawCount;

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	// インスタンス数カウント用定数バッファ
	instanceCountBuffer_ = std::make_unique<ConstantBuffer<int32_t>>(); // 生成
	instanceCountBuffer_->Init(device);									// 初期化
	*instanceCountBuffer_->data_ = 0;									// データ代入
	// トランスフォーム用のバッファを生成
	transformsBuffer_ = std::make_unique<StructuredBuffer<Matrix4x4>>(maxDrawCount_); // 生成
	transformsBuffer_->Init(device, srv);											  // 初期化
	// マテリアル用のバッファを生成
	materialsBuffer_ = std::make_unique<StructuredBuffer<MaterialData>>(maxDrawCount_); // 生成
	materialsBuffer_->Init(device, srv);											    // 初期化
}

void ParticleModel::AddDrawModel(const Matrix4x4& matWorld, const MaterialData& material)
{
	// 配列に情報追加
	transforms_.push_back(matWorld); // トランスフォーム
	materials_.push_back(material);	 // マテリアル
}

void ParticleModel::DisplayImGui()
{
	// ツリーノード開始
	if (ImGui::TreeNode(modelName_.c_str())) {

		// モデル表示トリガー
		ImGui::Checkbox("IsActive", &isActive_);

		// ツリーノード終了
		ImGui::TreePop();
	}
}
