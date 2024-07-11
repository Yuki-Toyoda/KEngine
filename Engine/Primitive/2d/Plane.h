#pragma once
#include "../IPrimitive.h"

/// <summary>
/// 平面クラス
/// </summary>
class Plane final : public IPrimitive
{
public: // メンバ関数

	/// <summary>
	/// コンストラクタに関しては形状基底クラスのを使用
	/// </summary>
	using IPrimitive::IPrimitive;

	/// <summary>
	/// (呼び出し禁止)頂点配列を形状の頂点数にリサイズする関数
	/// </summary>
	void ResizeVertices() override;

	/// <summary>
	/// 頂点数ゲッター
	/// </summary>
	/// <returns>頂点数</returns>
	int GetVertexCount() const { return 4; }

};

