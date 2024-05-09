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

	// アニメーションを１つでも所持している場合
	if (scene->mNumAnimations != 0) {
		// 全アニメーションをロード
		LoadAnimations(*scene);
	}

	// シーンのRootNodeを読み、シーン全体の階層構造を作る
	transform_->rootNode_ = ReadNode(scene->mRootNode);

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

WorldTransform::Node Mesh::ReadNode(aiNode* node)
{
	// ノード読み込み結果確認用
	WorldTransform::Node result;

	// ローカル行列を取得
	aiMatrix4x4 aiLocalMatrix = node->mTransformation; // ノードのlocalMatrixを取得
	aiLocalMatrix.Transpose();						   // 列ベクトル形式を行ベクトル形式に変換

	// 他の要素を同様に変換する
	result.localMatrix.m[0][0] = aiLocalMatrix[0][0];
	result.localMatrix.m[0][1] = aiLocalMatrix[0][1];
	result.localMatrix.m[0][2] = aiLocalMatrix[0][2];
	result.localMatrix.m[0][3] = aiLocalMatrix[0][3];
	result.localMatrix.m[1][0] = aiLocalMatrix[1][0];
	result.localMatrix.m[1][1] = aiLocalMatrix[1][1];
	result.localMatrix.m[1][2] = aiLocalMatrix[1][2];
	result.localMatrix.m[1][3] = aiLocalMatrix[1][3];
	result.localMatrix.m[2][0] = aiLocalMatrix[2][0];
	result.localMatrix.m[2][1] = aiLocalMatrix[2][1];
	result.localMatrix.m[2][2] = aiLocalMatrix[2][2];
	result.localMatrix.m[2][3] = aiLocalMatrix[2][3];
	result.localMatrix.m[3][0] = aiLocalMatrix[3][0];
	result.localMatrix.m[3][1] = aiLocalMatrix[3][1];
	result.localMatrix.m[3][2] = aiLocalMatrix[3][2];
	result.localMatrix.m[3][3] = aiLocalMatrix[3][3];

	// Node名を取得
	result.name = node->mName.C_Str();

	// 子ノード分配列スペースを確保
	result.children.resize(node->mNumChildren);

	// 子ノード数分ループ
	for (uint32_t childIndex = 0; childIndex < node->mNumChildren; childIndex++) {
		// 再帰的にノードを読み込み、階層構造を作る
		result.children[childIndex] = ReadNode(node->mChildren[childIndex]);
	}

	// ノードを所持している状態にする
	result.hasNode = true;

	// 読み込んだNode情報を返す
	return result;
}


void Mesh::LoadAnimations(const aiScene& scene)
{
	// 読み取り結果格納用配列
	std::vector<Animation> animations;

	// 全アニメーション分ループ
	for (uint32_t animationCount = 0; animationCount < scene.mNumAnimations; animationCount++) {
		// アニメーション生成
		Animation animation;
		// 最初のアニメーションのみを読み取る
		aiAnimation* animationAssimp = scene.mAnimations[animationCount];
		// 時間の単位を秒に変換
		animation.duration = float(animationAssimp->mDuration / animationAssimp->mTicksPerSecond);

		// assimpでは個々のNodeAnimationをchannelと呼んでいるので、channelを回してNodeAnimationの情報を取ってくる
		for (uint32_t channelIndex = 0; channelIndex < animationAssimp->mNumChannels; channelIndex++) {
			// ノードアニメーションを取得
			aiNodeAnim* nodeAnimationAssimp = animationAssimp->mChannels[channelIndex];
			// ノードアニメーションを任意の構造体の形式に変換
			NodeAnimation& nodeAnimation = animation.nodeAnimations[nodeAnimationAssimp->mNodeName.C_Str()];

			// 位置座標のキー数分ループ
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumPositionKeys; keyIndex++) {
				// 位置座標のキー情報を取得する
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mPositionKeys[keyIndex];
				// 位置座標のキーフレーム
				keyFrameVector3 keyFrame;

				// キーフレーム秒数を取得する
				keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
				// キーフレーム値を取得(右手から左手座標系に変換する)
				keyFrame.value = { -keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };

				// ノードアニメーション配列に値を追加
				nodeAnimation.translate.keyFrames.push_back(keyFrame);
			}

			// 回転のキー数分ループ
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumRotationKeys; keyIndex++) {
				// 回転のキー情報を取得する
				aiQuatKey& keyAssimp = nodeAnimationAssimp->mRotationKeys[keyIndex];
				// 回転のキーフレーム
				KeyFrameQuaternion keyFrame;

				// キーフレーム秒数を取得する
				keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
				// キーフレーム値を取得(右手から左手座標系に変換する)
				keyFrame.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z, keyAssimp.mValue.w };

				// ノードアニメーション配列に値を追加
				nodeAnimation.rotate.keyFrames.push_back(keyFrame);
			}

			// 拡縮のキー数分ループ
			for (uint32_t keyIndex = 0; keyIndex < nodeAnimationAssimp->mNumScalingKeys; keyIndex++) {
				// 拡縮のキー情報を取得する
				aiVectorKey& keyAssimp = nodeAnimationAssimp->mScalingKeys[keyIndex];
				// 拡縮のキーフレーム
				keyFrameVector3 keyFrame;

				// キーフレーム秒数を取得する
				keyFrame.time = float(keyAssimp.mTime / animationAssimp->mTicksPerSecond);
				// キーフレーム値を取得(右手から左手座標系に変換する)
				keyFrame.value = { keyAssimp.mValue.x, keyAssimp.mValue.y, keyAssimp.mValue.z };

				// ノードアニメーション配列に値を追加
				nodeAnimation.scale.keyFrames.push_back(keyFrame);
			}
		}

		// 解析を完了し次第、配列に追加する
		animations.push_back(animation);
	}

	// ワールドトランスフォームにアニメーションを渡す
	transform_->animations_ = animations;
}

