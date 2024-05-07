#include "Mesh.h"
#include "../../Resource/Texture/TextureManager.h"
#include "MeshManager.h"
#include "../../Base/DescriptorHeaps/SRV.h"

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

	// 最後にインデックスを反転させる（左手座標系に修正するため）
	for (size_t i = 0; i < indexes_.size(); i += 3) {
		// 0番目と2番目の要素を入れ替える
		std::swap(indexes_[i], indexes_[i + 2]);
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
