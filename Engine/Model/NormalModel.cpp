#include "NormalModel.h"
#include "Engine/Base/DirectXCommon.h"
#include "Engine/Resource/Texture/TextureManager.h"

void NormalModel::Init(ModelData* modelData)
{
	// 基底クラスの初期化処理を行う
	IModel::Init(modelData);
	// マテリアルの取得
	materials_ = modelData->materials_;
	// Dissolve用テクスチャのサンプルを読み込んでおく
	DissolveTex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "noise0.png");

	// トランスフォームの初期化
	transform_.Init();

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV*		   srv	  = DirectXCommon::GetInstance()->GetSRV();

	// トランスフォーム用のバッファ生成
	transformBuffer_ = std::make_unique<ConstantBuffer<Matrix4x4>>(); // 生成
	transformBuffer_->Init(device);									  // 初期化
	// マテリアル用のバッファを生成
	materialsBuffer_ = std::make_unique<StructuredBuffer<MaterialData>>(static_cast<int32_t>(materials_.size())); // 生成
	materialsBuffer_->Init(device, srv);																		  // 初期化
}

void NormalModel::Update()
{
	// マテリアルデータ変換保持用配列
	std::vector<MaterialData> materials;
	// 全マテリアルをマテリアルデータ構造体の変換
	for (int i = 0; i < materials_.size(); i++) {
		materials.push_back(materials_[i]);
	}

	// トランスフォームデータのコピー
	*transformBuffer_->data_ = transform_.GetMatWorld();
	// マテリアルデータのコピー
	std::memcpy(materialsBuffer_->data_, materials.data(), sizeof(MaterialData) * materials.size());
}

void NormalModel::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 描画用バッファをコマンドリストにセットする
	cmdList->SetGraphicsRootConstantBufferView(2, transformBuffer_->GetGPUView());					   // トランスフォーム
	cmdList->SetGraphicsRootDescriptorTable(3,  modelData_->meshletBuffer_->GetGPUView());			   // メッシュレット情報
	cmdList->SetGraphicsRootDescriptorTable(4,  modelData_->vertexBuffer_->GetGPUView());			   // 頂点情報
	cmdList->SetGraphicsRootDescriptorTable(5,  modelData_->uniqueVertexIndicesBuffer_->GetGPUView()); // 固有頂点情報
	cmdList->SetGraphicsRootDescriptorTable(6,  modelData_->primitiveIndicesBuffer_->GetGPUView());	   // プリミティブインデックス情報
	cmdList->SetGraphicsRootDescriptorTable(7,  materialsBuffer_->GetGPUView());					   // マテリアル
	cmdList->SetGraphicsRootDescriptorTable(8,  DissolveTex_.GetAddress());							   // Dissolve用テクスチャ

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(modelData_->GetMeshletCount(), 1, 1);
}

void NormalModel::DisplayImGui()
{
	// ツリーノード開始
	if (ImGui::TreeNode(modelName_.c_str())) {
		
		// モデル表示トリガー
		ImGui::Checkbox("IsActive", &isActive_);

		// マテリアル関連ImGui
		ImGui::Text("Materials");
		// 全マテリアルのImGuiを描画
		for (Material& m : materials_) {
			if (m.name_ != "") {
				m.DisplayImGui();
			}
		}
		// ツリーノード終了
		ImGui::TreePop();
	}
}
