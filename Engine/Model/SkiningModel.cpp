#include "SkiningModel.h"
#include "../Base/DirectXCommon.h"
#include "../Resource/Texture/TextureManager.h"

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
	// Dissolve用テクスチャのサンプルを読み込んでおく
	DissolveTex_ = TextureManager::Load("./Engine/Resource/Samples/Texture", "noise0.png");

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

	// 頂点数カウント用のバッファ生成
	vertexCountBuffer_ = std::make_unique<ConstantBuffer<int32_t>>();				 // 生成
	vertexCountBuffer_->Init(device);												 // 初期化
	*vertexCountBuffer_->data_ = static_cast<uint32_t>(modelData->vertices_.size()); // 代入
	// MatrixPaletteの生成
	palletteBuffer_ = std::make_unique<StructuredBuffer<WellForGPU>>(static_cast<uint32_t>(skelton_.joints_.size())); // 生成
	palletteBuffer_->Init(device, srv);																				  // 初期化
	// 頂点用バッファ生成
	vertexBuffer_ = std::make_unique<RWStructuredBuffer<VertexData>>(static_cast<uint32_t>(modelData->vertices_.size())); // 生成
	vertexBuffer_->Init(device, srv);																					  // 初期化
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

	// 親子関係データがある分ループ
	for (ParentData& p : parentDatas_) {
		// ボーンの行列とワールド行列を掛け合わせる
		p.localMat_ = GetBoneMatrix(p.boneName_);
		// トランスフォームに計算したワールドトランスフォームにセット
		p.transform_->SetWorldMat(&p.localMat_);
	}

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

void SkiningModel::ExecuteComputeSkining(ID3D12GraphicsCommandList6* cmdList)
{
	// 計算シェーダー用バッファをコマンドリストにセットする
	cmdList->SetComputeRootConstantBufferView(0, vertexCountBuffer_->GetGPUView());		// 頂点数
	cmdList->SetComputeRootDescriptorTable(1, palletteBuffer_->GetGPUView());			// マトリックスパレット
	cmdList->SetComputeRootDescriptorTable(2, modelData_->vertexBuffer_->GetGPUView()); // 頂点
	cmdList->SetComputeRootDescriptorTable(3, vertexBuffer_->GetUAVView());				// スキニング結果格納用

	// 計算シェーダーを実行する
	cmdList->Dispatch(UINT(modelData_->vertices_.size() + 1023) / 1024, 1, 1);
}

void SkiningModel::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 描画用バッファをコマンドリストにセットする
	cmdList->SetGraphicsRootConstantBufferView(2, transformBuffer_->GetGPUView());					  // トランスフォーム
	cmdList->SetGraphicsRootDescriptorTable(3, modelData_->meshletBuffer_->GetGPUView());			  // メッシュレット情報
	cmdList->SetGraphicsRootDescriptorTable(4, vertexBuffer_->GetSRVView());						  // 頂点情報
	cmdList->SetGraphicsRootDescriptorTable(5, modelData_->uniqueVertexIndicesBuffer_->GetGPUView()); // 固有頂点情報
	cmdList->SetGraphicsRootDescriptorTable(6, modelData_->primitiveIndicesBuffer_->GetGPUView());	  // プリミティブインデックス情報
	cmdList->SetGraphicsRootDescriptorTable(7, materialsBuffer_->GetGPUView());						  // マテリアル
	cmdList->SetGraphicsRootDescriptorTable(8, DissolveTex_.GetAddress());							  // Dissolve用テクスチャ

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(modelData_->GetMeshletCount(), 1, 1);
}

void SkiningModel::DisplayImGui()
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

		// アニメーションのImGuiを表示
		animationManager_.DisplayImGui();

		// ツリーノード終了
		ImGui::TreePop();
	}
}

void SkiningModel::SetBoneParent(const std::string boneName, WorldTransform* transform)
{
	// 指定されたボーン名称が存在するかどうかを確認する
	if (!skelton_.jointMap_.count(boneName)) { // 存在しない場合
		// ボーンが存在しないためログに出して停止
		Debug::Log("Bone Not Found.");
		assert(false);
	}

	// 新規親子関係データ生成
	ParentData newData;
	newData.transform_ = transform;
	newData.boneName_  = boneName;

	// 配列にデータを追加
	parentDatas_.push_back(newData);
}

Matrix4x4 SkiningModel::GetBoneMatrix(const std::string boneName)
{
	// 指定されたボーン名称が存在するかどうかを確認する
	if (!skelton_.jointMap_.count(boneName)) { // 存在しない場合
		// ボーンが存在しないためログに出して停止
		Debug::Log("Bone Not Found.");
		assert(false);
	}

	// 配列上の番号を取得する
	int boneIndex = skelton_.jointMap_[boneName];
	// 配列から情報取得
	return skelton_.joints_[boneIndex].skeltonSpaceMatrix_;
}

Vector3 SkiningModel::GetBonePosition(const std::string boneName)
{
	// 指定されたボーン名称が存在するかどうかを確認する
	if (!skelton_.jointMap_.count(boneName)) { // 存在しない場合
		// ボーンが存在しないためログに出して停止
		Debug::Log("Bone Not Found.");
		assert(false);
	}

	// 配列上の番号を取得する
	int boneIndex = skelton_.jointMap_[boneName];
	// 配列から情報取得
	return  Vector3{ skelton_.joints_[boneIndex].skeltonSpaceMatrix_.m[3][0], skelton_.joints_[boneIndex].skeltonSpaceMatrix_.m[3][1], skelton_.joints_[boneIndex].skeltonSpaceMatrix_.m[3][2] };
}
