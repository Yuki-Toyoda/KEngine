#include "ModelData.h"
#include "../../Base/DirectXCommon.h"

void ModelData::Load(const std::string& filePath, const std::string& fileName)
{
	// モデル名取得
	modelName_ = fileName;

	// フルパスの合成
	std::string fullPath = filePath + "/" + fileName;

	// assimpを読むためのimporterを定義
	Assimp::Importer importer;
	// importerでファイルを読み込む
	const aiScene* scene = importer.ReadFile(fullPath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	// メッシュを持っていないモデルの場合停止させる
	assert(scene->HasMeshes());

	// メッシュレット生成用情報の一時変数
	std::vector<VertexData>	  vertices;  // 頂点
	std::vector<uint32_t>	  indexes;	 // インデックス
	std::vector<MaterialData> materials; // マテリアル

	// 親ノードがある場合スケルトンとスキンクラスターの生成
	if (scene->mRootNode) {
		// 新規ノードの生成
		nodes_.emplace_back();
		// ノード情報の読み込み
		nodes_.back().Load(scene->mRootNode);

		// 新規スケルトンの追加
		skelton_.emplace();
		// ノード情報を元に生成
		skelton_->Create(nodes_[0]);

		// 生成したスケルトン情報を元にスキンクラスターを生成
		skinCluster_.emplace(static_cast<uint32_t>(skelton_->joints_.size()));

		// アニメーションの読み込み
		animationManager_.LoadAnimations(scene);
	}

	// メッシュの解析
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
		// assimpからメッシュ情報を取得
		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 新規メッシュを追加
		meshes_.emplace_back();
		// 読み込み用に追加したメッシュを取得
		Mesh& m = meshes_.back();

		// スケルトンが生成されている場合
		if (skelton_.has_value()) {
			// スケルトン、スキンクラスターを含めた読み込みを行う
			m.Load(mesh, skelton_.value(), skinCluster_.value());
		}
		else {
			// 通常の読み込みを行う
			m.Load(mesh);
		}

		// 現在読み込み中の頂点数を保持
		int vertexCount = static_cast<int>(vertices.size());
		// 全頂点を配列内に格納する
		for (int i = 0; i < m.vertices_.size(); i++) {
			// 頂点情報を配列に追加
			vertices.push_back(m.vertices_[i]);
		}
		// 全インデックス情報も配列内に格納する
		for (int i = 0; i < m.indexes_.size(); i++) {
			// インデックス情報を配列に追加
			indexes.push_back(m.indexes_[i] + vertexCount);
		}
	}

	// マテリアルの解析
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++) {
		// 新規マテリアルを追加
		materials_.emplace_back();
		// マテリアル情報の読み込み
		materials_.back().LoadMaterial(scene->mMaterials[materialIndex], filePath);
	}
	// マテリアルデータの保持
	for (int i = 0; i < materials_.size(); i++) {
		materials.push_back(materials_[i]);
	}

	// 頂点座標格納用配列の生成
	auto vertPos = std::make_unique<DirectX::XMFLOAT3[]>(vertices.size());
	// 頂点座標の格納
	for (int i = 0; i < vertices.size(); i++) {
		// 各頂点の情報を格納する
		vertPos[i] = { vertices[i].position_.x, vertices[i].position_.y, vertices[i].position_.z };
	}

	// メッシュレット生成後の一時変数
	std::vector<DirectX::Meshlet>		  meshlets;			// メッシュレット
	std::vector<uint8_t>				  uniqueVertices;	// 固有頂点
	std::vector<DirectX::MeshletTriangle> primitiveIndices; // プリミティブインデックス

	// メッシュレットの変換成否確認用
	HRESULT result = S_FALSE;
	// 読み込んだモデルデータをメッシュレットに変換する
	result = DirectX::ComputeMeshlets(
		indexes.data(), indexes.size() / 3,
		vertPos.get(), vertices.size(),
		nullptr,
		meshlets,
		uniqueVertices,
		primitiveIndices
	);
	// 変換成否確認
	assert(SUCCEEDED(result));

	// メッシュレットの数を保持
	meshletCount_ = static_cast<int>(meshlets.size());

	// バッファ生成用にデバイスとSRVを取得
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice(); // デバイス
	SRV*		   srv	  = DirectXCommon::GetInstance()->GetSRV();			  // SRV

	// バッファ生成
	meshletBuffer_			   = std::make_unique<StructuredBuffer<DirectX::Meshlet>>(static_cast<int32_t>(meshlets.size()));
	vertexBuffer_			   = std::make_unique<StructuredBuffer<VertexData>>(static_cast<int32_t>(vertices.size())); 
	uniqueVertexIndicesBuffer_ = std::make_unique<StructuredBuffer<uint32_t>>(static_cast<int32_t>(uniqueVertices.size()));
	primitiveIndicesBuffer_    = std::make_unique<StructuredBuffer<uint32_t>>(static_cast<int32_t>(primitiveIndices.size())); 
	materialsBuffer_		   = std::make_unique<StructuredBuffer<MaterialData>>(static_cast<int32_t>(materials.size()));

	// 初期化
	meshletBuffer_->Init(device, srv);
	vertexBuffer_->Init(device, srv);
	uniqueVertexIndicesBuffer_->Init(device, srv);
	primitiveIndicesBuffer_->Init(device, srv);
	materialsBuffer_->Init(device, srv);

	// データコピー
	std::memcpy(meshletBuffer_->data_, meshlets.data(), sizeof(DirectX::Meshlet) * meshlets.size());
	std::memcpy(vertexBuffer_->data_, vertices.data(), sizeof(VertexData) * vertices.size());
	std::memcpy(uniqueVertexIndicesBuffer_->data_, uniqueVertices.data(), sizeof(uint32_t) * uniqueVertices.size());
	std::memcpy(primitiveIndicesBuffer_->data_, primitiveIndices.data(), sizeof(uint32_t) * primitiveIndices.size());
	std::memcpy(materialsBuffer_->data_, materials.data(), sizeof(MaterialData) * materials.size());

}

std::vector<Vertex> ModelData::GetVertices() const
{
	// 返還用配列定義
	std::vector<Vertex> result;
	// 配列のリサイズ
	result.resize(GetVertexCount());

	// 全メッシュ分ループ
	for (const Mesh& m : meshes_) {
		result.insert(result.end(), m.vertices_.begin(), m.vertices_.end());
	}

	// 取得した結果を返す
	return result;
}

int ModelData::GetVertexCount() const
{
	// 返還用変数定義
	int size = 0;

	// 全メッシュ分ループ
	for (const Mesh& m : meshes_) {
		size += m.GetVertexCount();
	}

	// 取得した結果を返す
	return size;
}
