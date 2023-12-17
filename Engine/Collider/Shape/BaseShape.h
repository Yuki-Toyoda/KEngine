#pragma once
#include <vector>
#include "../../Math/Math.h"
#include "../../../Externals/imgui/imgui.h"

/// <summary>
/// コライダー形状の基底クラス
/// </summary>
class BaseShape {
public: // サブクラス

	/// <summary>
	/// コライダー形状列挙子
	/// </summary>
	enum ColliderShape {
		Sphere, // 球
		AABB, // AABB
		OBB // OBB
	};

public: // メンバ関数

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~BaseShape() {};

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	virtual void DisplayImGui() = 0;

public: // アクセッサ等

	/// <summary>
	/// コライダー形状のゲッター
	/// </summary>
	/// <returns>コライダー形状</returns>
	ColliderShape GetColliderShape() { return colliderShape_; }
	/// <summary>
	/// コライダー形状のセッター
	/// </summary>
	/// <param name="colliderShape">設定したいコライダー形状</param>
	void SetColliderShape(ColliderShape colliderShape) { colliderShape_ = colliderShape; }

public: // 共用関数群

	/// <summary>
	/// 中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	virtual const Vector3 GetCenter() { return Vector3(0.0f, 0.0f, 0.0f); }

public: // 球用関数群

	/// <summary>
	/// 球の当たり判定半径ゲッター
	/// </summary>
	/// <returns>球の当たり判定半径</returns>
	virtual const float GetRadius() { return 0.0f; }

public: // AABB用関数群

	/// <summary>
	/// AABB最小点ゲッター
	/// </summary>
	/// <returns>AABB最小点</returns>
	virtual const Vector3 GetMin() { return Vector3(0.0f, 0.0f, 0.0f); }
	/// <summary>
	/// AABB最大点ゲッター
	/// </summary>
	/// <returns>AABB最大点</returns>
	virtual const Vector3 GetMax() { return Vector3(0.0f, 0.0f, 0.0f); }

public: // OBB用関数群

	/// <summary>
	/// 座標軸ゲッター
	/// </summary>
	/// <returns>座標軸</returns>
	virtual const std::vector<Vector3> GetOtientatuons() { return std::vector<Vector3>(); }
	
	/// <summary>
	/// 座標軸の半分の長さゲッター
	/// </summary>
	/// <returns></returns>
	virtual const Vector3 GetSize() { return Vector3(0.0f, 0.0f, 0.0f); }

protected: // メンバ関数

	// コライダー形状
	ColliderShape colliderShape_;

};
