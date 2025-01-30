#pragma once
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Matrix4x4.h"
#include "Engine/Math/Quaternion.h"
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// <summary>
/// 各頂点のウェイト構造体
/// </summary>
struct VertexWeight {
	// 頂点のウェイト
	float weight_;
	// 影響を受ける頂点のインデックス
	uint32_t vertexIndex_;
};

/// <summary>
/// ジョイントのウェイトクラス
/// </summary>
class JointWeight
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	JointWeight() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~JointWeight() = default;

public: // メンバ関数

	/// <summary>
	/// assimpで読み込んだボーン情報を読み込む関数
	/// </summary>
	/// <param name="bone">読み込むボーン</param>
	void Load(aiBone* bone);

public: // パブリックメンバ変数

	// バインドポーズの逆行列
	Matrix4x4 inverseBindPoseMatrix_;
	// ジョイント内の頂点のウェイト配列
	std::vector<VertexWeight> vertexWeights_;

};

