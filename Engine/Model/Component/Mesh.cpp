#include "Mesh.h"

void Mesh::Load(aiMesh* mesh)
{
	// メッシュ名の取得
	name_ = mesh->mName.C_Str();

	// テクスチャ座標系があるか取得
	bool hasTexCoord = mesh->HasTextureCoords(0);
	// 法線があるかどうか取得
	bool hasNormal = mesh->HasNormals();

	// マテリアル番号の取得
	materialIndex_ = mesh->mMaterialIndex;

	// 頂点情報の解析
	for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
		// 新規頂点生成
		Vertex v;

		// インデックスを元に頂点情報取得
		aiVector3D& position = mesh->mVertices[vertexIndex];			// 取得
		v.position_			 = { -position.x, position.y, position.z }; // 代入

		// テクスチャ座標情報を取得
		if (hasTexCoord) { // テクスチャ座標系があるかどうか
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex]; // 取得
			v.texCoord_			 = { texcoord.x, texcoord.y };			 // 代入
		}
		else {
			// 0, 0 で代入
			v.texCoord_ = { 0.0f, 0.0f };
		}

		// 法線情報を取得
		if (hasNormal) { // 法線があるかどうか
			aiVector3D& normal = mesh->mNormals[vertexIndex]; // 取得
			v.normal_ = { -normal.x, normal.y, normal.z };	  // 代入	
		}
		else {
			// 頂点座標をそのまま代入
			v.normal_ = v.position_;
		}

		// 使用するマテリアル番号を頂点に保持する
		v.materialIndex_ = materialIndex_;

		// 頂点情報追加
		vertices_.push_back(v);
	}

	// 面情報の解析
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

	// ボーン情報の解析
	for (uint32_t boneIndex = 0; mesh->mNumBones; boneIndex++) {
		// ボーンを取得
		aiBone* bone = mesh->mBones[boneIndex];
		// ジョイント名取得
		std::string jointName = bone->mName.C_Str();
		// そのジョイント名のスキンクラスター用データを取得
		JointWeight& jointWeightData = skinClusterData_[jointName];

		// バインドポーズ保存用の4x4行列をAssimpから取得
		aiMatrix4x4 bindPoseMatrixAssimp = bone->mOffsetMatrix.Inverse();
		// 保存用変数を定義
		aiVector3D scale, translate;
		aiQuaternion rotate;

		// バインドポーズ行列からSRTを抽出
		bindPoseMatrixAssimp.Decompose(scale, rotate, translate);

		// 抽出したSRTからアフィン変換行列の生成
		Matrix4x4 bindPoseMatrix = Quaternion::MakeAffine(
			Vector3(scale.x, scale.y, scale.z),
			Quaternion(rotate.x, -rotate.y, -rotate.z, rotate.w),
			Vector3(-translate.x, translate.y, translate.z)
		);
		// 求めたアフィン変換行列の逆行列を求める
		jointWeightData.inverseBindPoseMatrix_ = Matrix4x4::MakeInverse(bindPoseMatrix);

		// ウェイト情報を解析する
		for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
			// ウェイト情報を追加
			jointWeightData.vertexWeights_.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
		}
	}
}

void Mesh::Load(aiMesh* mesh, Skelton& skelton, SkinCluster& skinCluster)
{
	// メッシュの読み込みを行う
	Load(mesh);

	// Modelのスキンクラスター情報を解析する
	for (const auto& jointWeight : skinClusterData_) {
		// ジョイント名でマップ内を捜索
		auto it = skelton.jointMap_.find(jointWeight.first);

		// マップ内当該ジョイントが存在しない場合
		if (it == skelton.jointMap_.end()) {
			// 次へ
			continue;
		}

		// イテレータのsecondにはジョイントのインデックスが入っているため、該当インデックスに行列を代入
		skinCluster.inverseBindPoseMatrices_[(*it).second] = jointWeight.second.inverseBindPoseMatrix_;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights_) {
			// 現在のインフルエンスを取得
			auto& currentInfluence = vertices_[vertexWeight.vertexIndex_];
			for (uint32_t index = 0; index < Vertex::kNumMaxInfluence; index++) {
				// weight == 0の場合空いているため、そこに代入する
				if (currentInfluence.weights_[index] == 0.0f) {
					// データを代入
					currentInfluence.weights_[index] = vertexWeight.weight_;
					currentInfluence.jointIndices_[index] = (*it).second;

					// 処理を抜ける
					break;
				}
			}
		}
	}
}
