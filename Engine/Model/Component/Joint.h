#pragma once
#include "Engine/GameObject/QuatWorldTransform.h"
#include "Node.h"

#include <vector>
#include <optional>
#include <string>

/// <summary>
/// ジョイントクラス
/// </summary>
class Joint
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Joint() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Joint() = default;

public: // 静的メンバ関数

	/// <summary>
	/// ジョイントの生成関数
	/// </summary>
	/// <param name="node"></param>
	/// <param name="parent">親ジョイント</param>
	/// <param name="joints">ジョイント配列</param>
	/// <returns></returns>
	static int32_t Create(const Node& node, const std::optional<int32_t>& parent, std::vector<Joint>& joints);

public: // アクセッサ等

	/// <summary>
	/// LocalMatrixゲッター
	/// </summary>
	/// <returns>LocalMatrix</returns>
	Matrix4x4 GetLocalMatrix() const;

public: // パブリックメンバ変数

	// ジョイント名
	std::string name_ = "";

	// トランスフォーム
	QuatWorldTransform transform_;

	// skeltonSpace上での変換行列
	Matrix4x4 skeltonSpaceMatrix_;

	// 子ジョイントインデックス配列
	std::vector<int32_t> children_;

	// 自身のインデックス
	int32_t index_;

	// 親ジョイントのインデックス
	std::optional<int32_t> parent_;

};
