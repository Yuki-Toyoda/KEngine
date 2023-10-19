#include "ModelManager.h"

ModelManager* ModelManager::GetInstance()
{
	static ModelManager modelManager;
	return &modelManager;
}

void ModelManager::Initialize()
{
	// 読み込んでいた全てのモデルを削除する
	modelDatas_.clear();
}

OBJ::ModelData ModelManager::LoadModel(const std::string& directoryPath, const std::string& fileName)
{
	// モデルまでのフルパスを合成する
	std::string FullPath = directoryPath + "/" + fileName;

	// 同名のキーを見つけたらそれを返す
	if (modelDatas_.find(FullPath) != modelDatas_.end()) {
		// モデルの頂点データ等を渡す
		return modelDatas_[FullPath];
	}
	else {// 見つからなかった場合は読み込む

		OBJ::ModelData modelData_; // 構築するモデルデータ
		std::vector<Vector4> positions_; // 位置
		std::vector<Vector2> uv_; // UV座標系
		std::vector<Vector3> normal_; // 法線
		std::string line_; // ファイルから読んだ１行を格納するもの

		// ファイルを開く
		std::ifstream file(FullPath);
		// 開けなかった場合を止める
		assert(file.is_open());

		// ファイルの終端まで読む
		while (std::getline(file, line_)) {
			// 先端の識別子の値を格納する変数
			std::string identifier;
			// ファイルの１行を取得
			std::istringstream s(line_);
			// 先頭の識別子を読む
			s >> identifier;

			// 頂点だった場合
			if (identifier == "v") {
				// 位置座標用
				Vector4 position;
				// 空文字区切りで１つずつ値を読む
				s >> position.x >> position.y >> position.z;
				// wを設定
				position.w = 1.0f;
				position.x *= -1.0f;
				// 頂点の配列に追加する
				positions_.push_back(position);
			}
			// UVだった場合
			else if (identifier == "vt") {
				// UV座標用
				Vector2 uv;
				// 空文字区切りで1つずつ値を読む
				s >> uv.x >> uv.y;
				uv.y = 1.0f - uv.y;
				// UVの配列に追加する
				uv_.push_back(uv);
			}
			// 法線だった場合
			else if (identifier == "vn") {
				// 法線用
				Vector3 normal;
				// 空文字区切りで１つずつ値を読む
				s >> normal.x >> normal.y >> normal.z;
				normal.x *= -1.0f;
				// 法線の配列に追加する
				normal_.push_back(normal);
			}
			// 終了なら
			else if (identifier == "f") {
				OBJ::VertexData triangle[3];
				// 面は三角形のみ、　そのほかは対応していない
				for (int32_t faceVertex = 0; faceVertex < 3; faceVertex++) {
					// 頂点情報
					std::string vertexDefinition;
					// 識別子を取得
					s >> vertexDefinition;

					// 頂点の要素へのIndexは 位置 / UV / 法線で格納されているため、分解してIndexを取得
					std::stringstream v(vertexDefinition);
					uint32_t elementIndices[3];
					for (int32_t element = 0; element < 3; element++) {
						std::string commitIndex;
						// 区切りのインデックスを読む
						std::getline(v, commitIndex, '/');
						elementIndices[element] = std::stoi(commitIndex);
					}
					// 要素へのIndexから実際の要素の値を取得して、頂点を構築する
					Vector4 position = positions_[elementIndices[0] - 1];
					Vector2 uv = uv_[elementIndices[1] - 1];
					Vector3 normal = normal_[elementIndices[2] - 1];
					triangle[faceVertex] = { position, uv, normal };
				}
				modelData_.vertices.push_back(triangle[2]);
				modelData_.vertices.push_back(triangle[1]);
				modelData_.vertices.push_back(triangle[0]);
			}
			else if (identifier == "mtllib") {
				std::string materialFileName;
				s >> materialFileName;
				modelData_.material = LoadMaterial(directoryPath, materialFileName);
			}
		}

		// 読み込んだモデルデータをコンテナに格納する
		modelDatas_[FullPath] = modelData_;

		// 値を返す
		return modelData_;

	}
}

OBJ::MaterialData ModelManager::LoadMaterial(const std::string& directoryPath, const std::string& fileName)
{
	OBJ::MaterialData materialData; // 構築するマテリアルデータ
	std::string line; // ファイルから読んだ１行を格納するモノ

	// ファイルを開く
	std::ifstream file(directoryPath + "/" + fileName);
	// 開けなかった場合を止める
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		// identifier に応じた処理
		if (identifier == "map_Kd") {
			std::string textureFileName;
			s >> textureFileName;
			// 連結してファイルパスに
			materialData.textureFilePath = textureFileName;
		}
	}

	// マテリアルデータを返す
	return materialData;
}
