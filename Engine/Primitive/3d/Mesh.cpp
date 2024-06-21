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

void Mesh::Draw(ID3D12GraphicsCommandList6* cmdList)
{
	// 変化する可能性のあるデータをコピー
	*transformBuffer_->data_ = transform_->GetMatWorld();

	// マテリアルのアップロード
	material_->UploadMaterial();

	// コマンドリストに各種バッファのアドレスをセット
	cmdList->SetGraphicsRootConstantBufferView(2, transformBuffer_->GetGPUView());		  // ワールドトランスフォーム
	cmdList->SetGraphicsRootConstantBufferView(3, material_->GetBufferAddress());		  // マテリアル
	cmdList->SetGraphicsRootDescriptorTable(4, meshletBuffer_->GetGPUView());			  // メッシュレット情報
	// スキンアニメーション用頂点バッファが存在する場合
	if (vertexSkinBuffer_ != nullptr) {
		cmdList->SetGraphicsRootDescriptorTable(5, vertexSkinBuffer_->GetGPUView());	  // スキンアニメーション用頂点情報
		cmdList->SetGraphicsRootDescriptorTable(8, skinCluster_.PalletteBuffer_->GetGPUView());	// テクスチャデータ
	}
	else {
		cmdList->SetGraphicsRootDescriptorTable(5, vertexBuffer_->GetGPUView());		  // 頂点情報
	}
	cmdList->SetGraphicsRootDescriptorTable(6, uniqueVertexIndicesBuffer_->GetGPUView()); // 固有頂点インデックス
	cmdList->SetGraphicsRootDescriptorTable(7, primitiveIndicesBuffer_->GetGPUView());	  // プリミティブインデックス

	// メッシュレットのプリミティブ数分メッシュシェーダーを実行
	cmdList->DispatchMesh(GetMeshletCount(), 1, 1);
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

		// 頂点情報を解析する
		for (uint32_t vertexIndex = 0; vertexIndex < mesh->mNumVertices; vertexIndex++) {
			// インデックス情報を元に情報を取得する
			aiVector3D& position = mesh->mVertices[vertexIndex];		 // 頂点座標取得
			aiVector3D& texcoord = mesh->mTextureCoords[0][vertexIndex]; // テクスチャ座標取得
			aiVector3D& normal	 = mesh->mNormals[vertexIndex];			 // 法線取得

			// 新規頂点データを追加
			Vertex newVertex;
			newVertex.position = { -position.x, position.y, position.z, 1.0f }; // 頂点座標追加
			newVertex.texCoord = { texcoord.x, texcoord.y };					// テクスチャ座標追加
			newVertex.normal = { -normal.x, normal.y, normal.z };				// 法線追加

			// アニメーションを１つでも所持している場合
			if (scene->mNumAnimations != 0) {
				// スキンアニメーション用の頂点データも追加する
				VertexSkin newSkinVertex;
				newSkinVertex.vertex = newVertex;

				// スキンアニメーション用の頂点データを追加
				skinVertices_.push_back(newSkinVertex);
			}

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

		// ボーン情報を解析する
		for (uint32_t boneIndex = 0; boneIndex < mesh->mNumBones; boneIndex++) {
			// ボーンを取得
			aiBone* bone = mesh->mBones[boneIndex];
			// ジョイント名取得
			std::string jointName = bone->mName.C_Str();
			// そのジョイント名のスキンクラスター用データを取得
			JointWeightData& jointWeightData = skinClusterData[jointName];

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
			jointWeightData.inverseBindPoseMatrix = Matrix4x4::MakeInverse(bindPoseMatrix);

			// ウェイト情報を解析する
			for (uint32_t weightIndex = 0; weightIndex < bone->mNumWeights; weightIndex++) {
				// ウェイト情報を追加
				jointWeightData.vertexWeights.push_back({ bone->mWeights[weightIndex].mWeight, bone->mWeights[weightIndex].mVertexId });
			}
		}
	}

	// マテリアルの解析を行う
	for (uint32_t materialIndex = 0; materialIndex < scene->mNumMaterials; materialIndex++) {
		if (material_->tex_.GetView_() == -1) {
			// マテリアル情報をシーンから取得する
			aiMaterial* material = scene->mMaterials[materialIndex];

			// マテリアル読み込み
			material_->LoadMaterial(material, filePath);
		}
		else {
			// 当該エンジンはマルチマテリアル対応をしていないのでテクスチャを読み込んだ時点でbreak
			break;
		}
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
	
	// アニメーションを１つでも所持している場合
	if (scene->mNumAnimations != 0) {
		// 全アニメーションをロード
		LoadAnimations(*scene);

		// アニメーションがある場合はこれらを生成する
		transform_->skelton_ = transform_->CreateSkelton(transform_->rootNode_); // スケルトン
		skinCluster_ = CreateSkinCluster(transform_->skelton_);					 // スキンクラスター

		// スキンアニメーション頂点用バッファの生成
		vertexSkinBuffer_ = std::make_unique<StructuredBuffer<VertexSkin>>(static_cast<int32_t>(skinVertices_.size())); // 生成
		vertexSkinBuffer_->Init(device, srv);																			// 初期化
		std::memcpy(vertexSkinBuffer_->data_, skinVertices_.data(), sizeof(VertexSkin)* skinVertices_.size());			// データコピー

		// アニメーションをするメッシュであることを伝える
		isAnimated_ = true;
	}
	else {
		// 頂点用バッファの生成
		vertexBuffer_ = std::make_unique<StructuredBuffer<Vertex>>(static_cast<int32_t>(vertices_.size())); // 生成
		vertexBuffer_->Init(device, srv);																	// 初期化
		std::memcpy(vertexBuffer_->data_, vertices_.data(), sizeof(Vertex)* vertices_.size());			    // データコピー
	}
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

	// ノードの位置情報格納用
	aiVector3D	 scale, translate;
	aiQuaternion rotate;

	// assimp の行列からSRTを抽出する関数を利用
	node->mTransformation.Decompose(scale, rotate, translate);
	// 抽出した要素を代入
	result.transform.scale = { scale.x, scale.y, scale.z };					 // 拡縮
	result.transform.rotate = { rotate.x, -rotate.y, -rotate.z, rotate.w };  // 回転
	result.transform.translate = { -translate.x, translate.y, translate.z }; // 位置
	// ローカル行列を求める
	result.localMatrix = Quaternion::MakeAffine(result.transform.scale, result.transform.rotate, result.transform.translate);

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


Mesh::SkinCluster Mesh::CreateSkinCluster(const WorldTransform::Skelton& skelton)
{
	// 返還用
	SkinCluster skinCluster;
	
	// バッファ生成のためにデバイスとSRVを取得する
	DirectXDevice* device = DirectXCommon::GetInstance()->GetDirectXDevice();
	SRV* srv = DirectXCommon::GetInstance()->GetSRV();

	// MatrixPaletteの生成
	skinCluster.PalletteBuffer_ = std::make_unique<StructuredBuffer<WellForGPU>>(static_cast<uint32_t>(skelton.joints.size()));
	skinCluster.PalletteBuffer_->Init(device, srv);

	// Inflenceの生成
	/*skinCluster.influencedBuffer_ = std::make_unique<StructuredBuffer<VertexInfluence>>(static_cast<uint32_t>(vertices_.size()));
	skinCluster.influencedBuffer_->Init(device, srv);*/

	// バインドポーズ逆行列を格納する場所を確保する
	skinCluster.inverseBindPoseMatrices.resize(skelton.joints.size());
	// 生成した場所を単位行列で埋める
	std::generate(skinCluster.inverseBindPoseMatrices.begin(), skinCluster.inverseBindPoseMatrices.end(), Matrix4x4::MakeIndentity);

	// Modelのスキンクラスター情報を解析する
	for (const auto& jointWeight : skinClusterData) {
		// ジョイント名でマップ内を捜索
		auto it = skelton.jointMap.find(jointWeight.first);

		// マップ内当該ジョイントが存在しない場合
		if (it == skelton.jointMap.end()) {
			// 次へ
			continue;
		}

		// イテレータのsecondにはジョイントのインデックスが入っているため、該当インデックスに行列を代入
		skinCluster.inverseBindPoseMatrices[(*it).second] = jointWeight.second.inverseBindPoseMatrix;
		for (const auto& vertexWeight : jointWeight.second.vertexWeights) {
			// 現在のインフルエンスを取得
			auto& currentInfluence = skinVertices_[vertexWeight.vertexIndex];
			for (uint32_t index = 0; index < kNumMaxInfluence; index++) {
				// weight == 0の場合空いているため、そこに代入する
				if (currentInfluence.weights[index] == 0.0f) {
					// データを代入
					currentInfluence.weights[index] = vertexWeight.weight;
					currentInfluence.jointIndices[index] = (*it).second;

					// 処理を抜ける
					break;
				}
			}
		}
	}

	// 生成したスキンクラスターを返す
	return skinCluster;
}

void Mesh::SkinClusterUpdate(SkinCluster& skinCluster, const WorldTransform::Skelton& skelton)
{
	// 全ジョイント数分ループ
	for (size_t jointIndex = 0; jointIndex < skelton.joints.size(); jointIndex++) {
		// インデックスが行列数を超過していた場合停止させる
		assert(jointIndex < skinCluster.inverseBindPoseMatrices.size());
		skinCluster.PalletteBuffer_->data_[jointIndex].skeltonSpaceMatrix =
			skinCluster.inverseBindPoseMatrices[jointIndex] * skelton.joints[jointIndex].skeltonSpaceMatrix;
		skinCluster.PalletteBuffer_->data_[jointIndex].skeltonSpaceTransposeMatrix =
			Matrix4x4::MakeTranspose(Matrix4x4::MakeInverse(skinCluster.PalletteBuffer_->data_[jointIndex].skeltonSpaceMatrix));
	}
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
				keyFrame.value = { keyAssimp.mValue.x, -keyAssimp.mValue.y, -keyAssimp.mValue.z, keyAssimp.mValue.w };

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

