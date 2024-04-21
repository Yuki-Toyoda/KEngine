#include "Mesh.h"
#include "../../Resource/Texture/TextureManager.h"
#include "MeshManager.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <map>
#include <fstream>
#include <sstream>

void Mesh::LoadFile(const std::string& filePath, const std::string& fileName)
{
	// 頂点情報とインデックス情報をクリア
	vertices_.clear(); // 頂点情報
	indexes_.clear();  // インデックス情報

	// objを読み込む
	LoadObj(filePath, fileName);
}

void Mesh::LoadObj(const std::string& filePath, const std::string& fileName)
{
	// フルパスの合成
	std::string fullPath = filePath + "/" + fileName;

	// メッシュマネージャーのインスタンスを取得
	MeshManager* meshManaer = MeshManager::GetInstance();

	// 頂点マップ内を探索し同名のファイルパスのメッシュが読み込まれているか確認
	if (meshManaer->meshVertexMap_.count(filePath)){
		// 読み込まれていた場合は各種情報を渡す
		vertices_ = meshManaer->meshVertexMap_[filePath]; // 頂点
		indexes_ = meshManaer->meshIndexMap_[filePath];	  // インデックス
	}
	else { // 読み込まれていなかった場合は読み込む

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
					// インデックス情報を元に情報を取得する
					aiVector3D& position = mesh->mVertices[vertexIndex];		 // 頂点座標取得
					aiVector3D& normal	 = mesh->mNormals[vertexIndex];			 // 法線取得
					aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex]; // テクスチャ座標取得

					// 左手座標系に変換
					position.x *= -1.0f;			// 頂点の向きを変換
					normal.x   *= -1.0f;			// 法線の向きを反転

					// 新規頂点データを追加
					Vertex newVertex;
					newVertex.position = { position.x, position.y, position.z }; // 頂点座標追加
					newVertex.texCoord = { texcoord.x, texcoord.y };			 // テクスチャ座標追加
					newVertex.normal   = { normal.x, normal.y, normal.z };		 // 法線追加

					// 頂点データを追加
					vertices_.push_back(newVertex);

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

				// マテリアル読み込み
				texture_ = TextureManager::Load(filePath, textureFilePath.C_Str());
			}
		}

		// 左手座標系に変換するため、インデックス情報を反転させる
		for (size_t i = 0; i < indexes_.size(); i += 3)
			std::swap(indexes_[i], indexes_[i + 2]); // 0番目と2番目の要素を入れ替え、反転させる


		// 今回読み込んだモデルのフルパスと各種情報を保存
		meshManaer->AddInfo(fullPath, vertices_, indexes_);
	}
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
			texture_ = TextureManager::Load(filePath, textureName);
		}
	}
}
