#include "Mesh.h"
#include "../../Resource/Texture/TextureManager.h"
#include "MeshManager.h"

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
		// インデックス情報を登録するための3次元配列
		std::map<int, std::map<int, std::map<int, int>>> key;

		// 頂点情報の格納ための変数生成
		std::vector<Vector3> positions; // 位置
		std::vector<Vector2> texcoords; // テクスチャ座標
		std::vector<Vector3> normals;	// 法線

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

			// 頂点データ登録
			if (identifier == "v") {
				Vector3 position;							 // 読み込み情報格納用
				s >> position.x >> position.y >> position.z; // その行から情報を読み込み
				positions.push_back(position);				 // 読み込んだ情報を配列を格納
			}
			// テクスチャ座標登録
			else if (identifier == "vt") {
				Vector2 texcoord;			   // 読み込み情報格納用
				s >> texcoord.x >> texcoord.y; // その行から情報を読み込み
				texcoords.push_back(texcoord); // 読み込んだ情報を配列を格納
			}
			// 法線登録
			else if (identifier == "vn") {
				Vector3 normal;						   // 読み込み情報格納用
				s >> normal.x >> normal.y >> normal.z; // その行から情報を読み込み
				normals.push_back(normal);			   // 読み込んだ情報を配列を格納
			}
			// 頂点とインデックス情報を登録
			else if (identifier == "f") {
				// 面は三角形限定であり、その他は対応していない
				for (int32_t faceVertex = 0; faceVertex < 3; faceVertex++) {
					std::string vertexDefinition; // 頂点インデックス情報取得用
					s >> vertexDefinition; // 現在の行の文字列を取得

					// 頂点要素へのインデックスは 位置 / UV / 法線 の順で登録されている
					// そのため、分解してIndexを取得する
					std::istringstream v(vertexDefinition); // 1頂点分のデータを取得
					std::string elements[3]; // 0 : 位置, 1 : UV, 2 : 法線
					for (int32_t i = 0; i < 3; i++) {
						std::getline(v, elements[i], '/'); // それぞれの要素を取得
					}

					// 実際に頂点を追加する前に追加する頂点が新しいデータかどうかチェック
					int indexInfo = key[std::stoi(elements[0])][std::stoi(elements[1])][std::stoi(elements[2])]; // インデックス情報を取得
					// 既存のパターンだった場合、既存のインデックス情報を取得、インデックスに追加
					if (indexInfo > 0 && indexInfo < vertices_.size())
						indexes_.push_back(static_cast<uint32_t> (indexInfo - 1)); // パターンが見つかった場合のインデックス
					// 新しいパターンの場合は新しく頂点を追加、インデックスも追加
					else {
						Vertex newVertex; // 新しい頂点の構造体を宣言
						newVertex.position = positions[std::stoi(elements[0]) - 1]; // 頂点
						newVertex.texCoord = texcoords[std::stoi(elements[1]) - 1]; // テクスチャ座標
						newVertex.normal = normals[std::stoi(elements[2]) - 1];		// 法線

						// 左手座標系に変換
						newVertex.position.x *= -1.0f;						// 頂点の向きを変換
						newVertex.texCoord.y = 1.0f - newVertex.texCoord.y; // テクスチャ座標を反転
						newVertex.normal.x *= -1.0f;						// 法線の向きを反転

						// 頂点を追加
						vertices_.push_back(newVertex);
						// インデックスを登録
						indexes_.push_back(static_cast<uint32_t> (vertices_.size() - 1));

						// 既存パターンであることを示すために現在のインデックス情報を既存インデックス情報マップに追加
						key[std::stoi(elements[0])][std::stoi(elements[1])][std::stoi(elements[2])] = static_cast<int>(vertices_.size());
					}
				}
			}
			// マテリアル情報読み込み
			else if (identifier == "mtllib") {
				// マテリアル名を取得
				std::string materialFileName; // マテリアル名格納用
				s >> materialFileName;		  // マテリアル名取得
				// マテリアル読み込み
				LoadMaterial(filePath, materialFileName);
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
