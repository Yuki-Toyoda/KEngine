#pragma once
#include "Vertex.h"
#include "JointWeight.h"
#include "Skelton.h"
#include "SkinCluster.h"
#include "../../../Base/Resource/Data/StructuredBuffer.h"

#include <vector>
#include <map>

/// <summary>
/// メッシュクラス
/// </summary>
class Mesh final
{
public: // メンバ関数

	/// <summary>
	/// assimpが読み込んだメッシュ情報を読み込む関数
	/// </summary>
	/// <param name="mesh">読み込むメッシュ</param>
	void Load(aiMesh* mesh);

	/// <summary>
	/// assimpが読み込んだメッシュ情報を読み込む関数
	/// </summary>
	/// <param name="mesh">読み込むメッシュ</param>
	/// <param name="skelton">スケルトン</param>
	/// <param name="skinCluster">スキンクラスター</param>
	void Load(aiMesh* mesh, Skelton& skelton, SkinCluster& skinCluster);

public: // アクセッサ等

	/// <summary>
	/// 頂点数ゲッター
	/// </summary>
	/// <returns>頂点数</returns>
	int GetVertexCount() const { return static_cast<int>(vertices_.size()); }
	
	/// <summary>
	/// インデックス数ゲッター
	/// </summary>
	/// <returns>頂点数</returns>
	int GetIndexCount() const { return static_cast<int>(indexes_.size()); }

public: // パブリックメンバ変数

	// メッシュ名
	std::string name_ = "Mesh";

	// 頂点配列
	std::vector<Vertex> vertices_;
	// インデックス配列
	std::vector<uint32_t> indexes_;
	// ジョイントのウェイト配列
	std::map<std::string, JointWeight> skinClusterData_;

	// 使用するマテリアル番号
	int materialIndex_ = 0;

};

