#include "Mesh.h"
#include "../../Resource/Texture/TextureManager.h"
#include "MeshManager.h"
#include "../../Base/DirectXCommon.h"

#include <map>
#include <fstream>
#include <sstream>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

void Mesh::LoadModelFile(const std::string& filePath, const std::string& fileName)
{
	// 頂点情報とインデックス情報をクリア
	vertices_.clear(); // 頂点情報
	indexes_.clear();  // インデックス情報

	// objを読み込む
	LoadModel(filePath, fileName);
}

void Mesh::LoadModel(const std::string& filePath, const std::string& fileName)
{
	// フルパスのフルパスの合成
	std::string fullPath = filePath + "/" + fileName;

	// assimpを読むためのimporterを定義
	Assimp::Importer importer;
	// importerでファイルを読み込む
	const aiScene* scene = importer.ReadFile(fullPath.c_str(), aiProcess_FlipWindingOrder | aiProcess_FlipUVs);
	// メッシュを持っていないモデルの場合停止させる
	assert(scene->HasMeshes());

	// 全メッシュを解析する
	for (uint32_t meshIndex = 0; meshIndex < scene->mNumMeshes; meshIndex++) {
		// メッシュを取得
		aiMesh* mesh = scene->mMeshes[meshIndex];

		// 法線がないメッシュは非対応
		assert(mesh->HasNormals());
		// テクスチャ座標系がないメッシュは非対応
		assert(mesh->HasTextureCoords(0));

		// 頂点数分ループ
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			// インデックス情報を元に情報を取得する
			aiVector3D& position = mesh->mVertices[vertexIndex];		 // 頂点座標取得
			aiVector3D& normal = mesh->mNormals[vertexIndex];			 // 法線取得
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex]; // テクスチャ座標取得

			// 新規頂点データを追加
			Vertex newVertex;
			newVertex.position = { -position.x, position.y, position.z, 1.0f }; // 頂点座標追加
			newVertex.texCoord = { texcoord.x, texcoord.y };					// テクスチャ座標追加
			newVertex.normal = { -normal.x, normal.y, normal.z };				// 法線追加

			// 頂点データを追加
			vertices_.push_back(newVertex);
		}

		// 面情報を解析する
		for (uint32_t faceIndex = 0; faceIndex < mesh->mNumFaces; faceIndex++) {
			// 面を取得する
			aiFace& face = mesh->mFaces[faceIndex];

			// 三角形以外だった場合
			assert(face.mNumIndices == 3);

			// 全インデックス数分loop
			for (uint32_t element = 0; element < face.mNumIndices; element++) {
				// 頂点インデックスを取得
				uint32_t vertexIndex = face.mIndices[element];

				// インデックス情報も追加
				indexes_.push_back(vertexIndex);
			}
		}
	}

	// マテリアルの解析を行う
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++) {
		// マテリアル情報をシーンから取得する
		aiMaterial* material = scene->mMaterials[materialIndex];

		if (material->GetTextureCount(aiTextureType_DIFFUSE) != 0) {
			// テクスチャまでのファイルパス格納用
			aiString textureFilePath;
			// テクスチャファイルのパスを取得
			material->GetTexture(aiTextureType_DIFFUSE, 0, &textureFilePath);

			// テクスチャの読み込み
			material_->tex_ = TextureManager::Load(filePath, textureFilePath.C_Str());
		}
		else { // テクスチャがない場合
			// デフォルトテクスチャ取得
			material_->tex_ = TextureManager::GetInstance()->GetDefaultTexture();
		}
	}


	// 頂点座標格納用配列の作成
	auto vertPos = std::make_unique<DirectX::XMFLOAT3[]>(vertices_.size());
	// 頂点座標を取得
	for (size_t j = 0; j < vertices_.size(); j++) {
		vertPos[j] = { vertices_[j].position.x, vertices_[j].position.y, vertices_[j].position.z };
	}

	// メッシュレットの変換成否確認用
	HRESULT result = S_FALSE;
	// 読み込んだモデルデータをメッシュレットに変換する
	result = DirectX::ComputeMeshlets(
		indexes_.data(), indexes_.size() / 3,
		vertPos.get(), vertices_.size(),
		nullptr,
		meshlets_,
		uniqueVertices_,
		primitiveIndices_
	);
	// 変換成否確認
	assert(SUCCEEDED(result));

	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	/// データ読み込みが完了した時点で各バッファを指定サイズで生成する
	// トランスフォーム用バッファの生成
	transformBuffer_ = std::make_unique<ConstantBuffer<Matrix4x4>>(); // 生成
	transformBuffer_->Init(device);									  // 初期化
	*transformBuffer_->data_ = transform_->GetMatWorld();			  // データの代入
	// メッシュレット用バッファの生成
	meshletBuffer_ = std::make_unique<StructuredBuffer<DirectX::Meshlet>>(static_cast<int32_t>(meshlets_.size())); // 生成
	meshletBuffer_->Init(device, srv);																			   // 初期化
	std::memcpy(meshletBuffer_->data_, meshlets_.data(), sizeof(DirectX::Meshlet) * meshlets_.size());			   // データコピー
	// 頂点用バッファの生成
	vertexBuffer_ = std::make_unique<StructuredBuffer<Vertex>>(static_cast<int32_t>(vertices_.size())); // 生成
	vertexBuffer_->Init(device, srv);																	// 初期化
	std::memcpy(vertexBuffer_->data_, vertices_.data(), sizeof(Vertex) * vertices_.size());			    // データコピー
	// 固有頂点インデックス用バッファの生成
	uniqueVertexIndicesBuffer_ = std::make_unique<StructuredBuffer<uint32_t>>(static_cast<int32_t>(uniqueVertices_.size()));   // 生成
	uniqueVertexIndicesBuffer_->Init(device, srv);																			   // 初期化
	std::memcpy(uniqueVertexIndicesBuffer_->data_, uniqueVertices_.data(), sizeof(uint8_t) * uniqueVertices_.size());		   // データコピー
	// プリミティブインデックス用バッファの生成
	primitiveIndicesBuffer_ = std::make_unique<StructuredBuffer<uint32_t>>(static_cast<int32_t>(primitiveIndices_.size()));				// 生成
	primitiveIndicesBuffer_->Init(device, srv);																							// 初期化
	std::memcpy(primitiveIndicesBuffer_->data_, primitiveIndices_.data(), sizeof(DirectX::MeshletTriangle) * primitiveIndices_.size());	// データコピー

}

void Mesh::LoadMaterial(const std::string& filePath, const std::string& fileName)
{
	// フルパスの合成
	std::string fullPath = filePath + "/" + fileName;
	// ファイルを開く
	std::ifstream file(fullPath); // ファイルを開く
	// 開けなかった場合を止める
	assert(file.is_open());

	// ファイルから読んだ１行を格納するもの
	std::string line;

	// ファイルから1行ずつ読み込む
	while (std::getline(file, line)) {
		// 先端の識別子の値を格納する変数
		std::string identifier;
		// ファイルの１行を取得
		std::istringstream s(line);
		// 先頭の識別子を読む
		s >> identifier;

		// テクスチャの読み込み
		if (identifier == "map_Kd") {
			// テクスチャ名取得
			std::string textureName; // テクスチャ情報格納用
			s >> textureName;		 // テクスチャ名取得
			// テクスチャ読み込み
			//texture_ = TextureManager::Load(filePath, textureName);
		}
	}
}
