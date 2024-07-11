#pragma once
#include <string>
#include "../../DirectXMesh/DirectXMesh.h"
#include "../../Base/Resource/Data/StructuredBuffer.h"

#include "../Component/Mesh.h"
#include "../Component/Node.h"
#include "../Animation/SkinAnimationManager.h"
#include "../../Resource/Material/Material.h"

/// <summary>
/// モデル実体
/// </summary>
class ModelData final
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ModelData() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~ModelData() = default;

public: // メンバ関数

	/// <summary>
	/// モデルロード関数
	/// </summary>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="fileName">モデルのファイル名</param>
	void Load(const std::string& filePath, const std::string& fileName);

public: // アクセッサ等

	/// <summary>
	/// 全頂点ゲッター
	/// </summary>
	/// <returns>全頂点</returns>
	std::vector<Vertex> GetVertices() const;

	/// <summary>
	/// 頂点数ゲッター
	/// </summary>
	/// <returns>頂点数</returns>
	int GetVertexCount() const;

	/// <summary>
	/// メッシュ数ゲッター
	/// </summary>
	/// <returns>メッシュ数</returns>
	int GetMeshCount() const { return static_cast<int>(meshes_.size()); }

	/// <summary>
	/// メッシュレット数ゲッター
	/// </summary>
	/// <returns>メッシュレット数</returns>
	int GetMeshletCount() const { return meshletCount_; }

public: // パブリックメンバ変数

	// モデル名
	std::string modelName_ = "";

	// 頂点データ格納用
	std::vector<VertexData>	vertices_;

	// モデルデータ配列群
	std::vector<Mesh>		   meshes_;		   // メッシュ
	std::vector<Node>		   nodes_;		   // ノード
	std::vector<Material>	   materials_;	   // マテリアル
	std::optional<Skelton>	   skelton_{};	   // スケルトン
	std::optional<SkinCluster> skinCluster_{}; // スキンクラスター

	// アニメーション管理マネージャ
	SkinAnimationManager animationManager_{};

	// バッファ変数群
	std::unique_ptr<StructuredBuffer<DirectX::Meshlet>> meshletBuffer_;				// メッシュレット
	std::unique_ptr<StructuredBuffer<VertexData>>		vertexBuffer_;				// 頂点
	std::unique_ptr<StructuredBuffer<uint32_t>>			uniqueVertexIndicesBuffer_; // 固有頂点インデックス
	std::unique_ptr<StructuredBuffer<uint32_t>>			primitiveIndicesBuffer_;	// プリミティブインデックス
	std::unique_ptr<StructuredBuffer<MaterialData>>		materialsBuffer_;			// マテリアル

private: // メンバ変数

	// メッシュレット数
	int meshletCount_ = 0;

};

 