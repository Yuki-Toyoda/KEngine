#pragma once
#include "../IPrimitive.h"
#include <array>
#include <span>
#include "../../Base/Resource/Data/StructuredBuffer.h"

/// <summary>
/// 3Dモデル
/// </summary>
class OldMesh final : public IPrimitive
{
public: // サブクラス

	// 頂点が影響を受けるジョイントの最大数
	static const uint32_t kNumMaxInfluence = 4;

	/// <summary>
	/// スキンアニメーション用頂点構造体
	/// </summary>
	struct VertexSkin {
		OldVertex vertex;
		std::array<float, kNumMaxInfluence> weights{};
		std::array<int32_t, kNumMaxInfluence> jointIndices{};
	};

	/// <summary>
	/// 頂点のウェイトデータ
	/// </summary>
	struct VertexWeightData {
		float weight;		  // ウェイト
		uint32_t vertexIndex; // 頂点インデックス
	};

	/// <summary>
	/// ジョイントのウェイトデータ
	/// </summary>
	struct JointWeightData {
		Matrix4x4 inverseBindPoseMatrix;
		std::vector<VertexWeightData> vertexWeights;
	};

	/// <summary>
	/// ウェイトが頂点に対して影響を受けるパラメーター用構造体
	/// </summary>
	struct VertexInfluence {
		std::array<float, kNumMaxInfluence> weights;
		std::array<int32_t, kNumMaxInfluence> jointIndices;
	};

	/// <summary>
	/// マトリックスのパレット
	/// </summary>
	struct WellForGPU {
		Matrix4x4 skeltonSpaceMatrix;
		Matrix4x4 skeltonSpaceTransposeMatrix;
	};

	/// <summary>
	/// スキンクラスター構造体
	/// </summary>
	struct SkinCluster {
		std::vector<Matrix4x4> inverseBindPoseMatrices;
		//std::unique_ptr<StructuredBuffer<VertexInfluence>> influencedBuffer_;
		std::unique_ptr<StructuredBuffer<WellForGPU>> PalletteBuffer_;
	};

public: // メンバ関数

	/// <summary>
	/// コンストラクタに関しては形状基底クラスのを使用
	/// </summary>
	using IPrimitive::IPrimitive;
	
	/// <summary>
	/// (呼び出し禁止)頂点配列を形状の頂点数にリサイズする関数
	/// </summary>
	void ResizeVertices() override { /*使用しないため記述なし*/ };
	/// <summary>
	/// (呼び出し禁止)インデックス情報配列を形状の情報をもとにリサイズする関数
	/// </summary>
	void ResizeIndexes() override { /*使用しないため記述なし*/ };

	/// <summary>
	/// モデルデータ読み込み関数
	/// </summary>
	/// <param name="filePath">モデルまでのディレクトリ</param>
	/// <param name="fileName">モデル名</param>
	void LoadModelFile(const std::string& filePath, const std::string& fileName);

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList6* cmdList) override;

public: // アクセッサ等

	/// <summary>
	/// 頂点数ゲッター
	/// </summary>
	/// <returns>頂点数</returns>
	int GetVertexCount() const override { return static_cast<int>(vertices_.size()); }
	/// <summary>
	/// インデックス情報数ゲッター
	/// </summary>
	/// <returns>インデックス情報の数</returns>
	int GetIndexCount() const override { return static_cast<int>(indexes_.size()); }

private: // プライベートなメンバ関数

	/// <summary>
	/// objの読み込み関数
	/// </summary>
	/// <param name="filePath">モデルまでのディレクトリ</param>
	/// <param name="fileName">モデル名</param>
	void LoadModel(const std::string& filePath, const std::string& filename);

public: // アニメーション関連関数

	/// <summary>
	/// assimpNodeからNode情報を読み取る関数
	/// </summary>
	/// <param name="node">ノード情報</param>
	/// <returns>ノード構造体</returns>
	WorldTransform::Node ReadNode(aiNode* node);

	/// <summary>
	/// スキンクラスター生成関数
	/// </summary>
	/// <param name="skelton">スケルトン</param>
	/// <returns>スキンクラスター</returns>
	SkinCluster CreateSkinCluster(const WorldTransform::Skelton& skelton);

	/// <summary>
	/// スキンクラスターの更新関数
	/// </summary>
	/// <param name="skinCluster">スキンクラスター</param>
	/// <param name="skelton">スケルトン</param>
	void SkinClusterUpdate(SkinCluster& skinCluster, const WorldTransform::Skelton& skelton);

	/// <summary>
	/// 全アニメーションロード関数
	/// </summary>
	/// <param name="scene">アニメーションをロードするシーン</param>
	void LoadAnimations(const aiScene& scene);

public: // パブリックなメンバ変数

	// スキンクラスター
	SkinCluster skinCluster_;

private: // メンバ変数

	// スキンアニメーション用頂点配列
	std::vector<VertexSkin> skinVertices_;
	// スキンアニメーション用頂点バッファ
	std::unique_ptr<StructuredBuffer<VertexSkin>> vertexSkinBuffer_;

	// スキンクラスター用データ
	std::map<std::string, JointWeightData> skinClusterData;

};

