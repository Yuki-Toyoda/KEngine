#include "ParticleModel.h"
#include "Engine/Base/DirectXCommon.h"

void ParticleModel::Init(ModelData* modelData)
{
	// 基底クラスの初期化処理を行う
	IModel::Init(modelData);
	// マテリアルの取得
	materials_ = modelData->materials_;

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	// マテリアル用バッファ生成
	materialsBuffer_ = std::make_unique<StructuredBuffer<MaterialData>>(static_cast<int32_t>(materials_.size())); // 生成
	materialsBuffer_->Init(device, srv);																		  // 初期化
}

void ParticleModel::Update()
{
	// マテリアルデータ変換保持用配列
	std::vector<MaterialData> materials;
	// 全マテリアルをマテリアルデータ構造体の変換
	for (int i = 0; i < materials_.size(); i++) {
		materials.push_back(materials_[i]);
	}

	// マテリアルデータのコピー
	std::memcpy(materialsBuffer_->data_, materials.data(), sizeof(MaterialData) * materials.size());
}

void ParticleModel::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	/*該当モデルでは処理を行わない*/
	cmdList; 
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
