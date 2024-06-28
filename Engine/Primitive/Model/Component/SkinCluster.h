#pragma once
#include "../../../Base/Resource/Data/StructuredBuffer.h"
#include "../../../GameObject/QuatWorldTransform.h"
#include "Node.h"

#include <vector>
#include <algorithm>

/// <summary>
/// スキンクラスタークラス
/// </summary>
class SkinCluster
{
public: // コンストラクタ

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkinCluster() = delete;
	/// <summary>
	/// コピーコンストラクタ
	/// </summary>
	/// <param name="size"></param>
	SkinCluster(uint32_t size);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkinCluster() = default;

public: // パブリックメンバ変数

	// 逆バインドポーズ行列配列
	std::vector<Matrix4x4> inverseBindPoseMatrices_;

};

