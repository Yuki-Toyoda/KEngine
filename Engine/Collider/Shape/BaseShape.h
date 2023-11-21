#pragma once
#include "../../Math/Math.h"

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
	virtual Vector3 GetCenter() { return Vector3(0.0f, 0.0f, 0.0f); }

public: // 球用関数群

	/// <summary>
	/// 更新関数(球)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="radius">半径</param>
	virtual void Update(Vector3 center, float radius) {
		center;
		radius;
	};

	/// <summary>
	/// 球の当たり判定半径ゲッター
	/// </summary>
	/// <returns>球の当たり判定半径</returns>
	virtual float GetRadius() { return 0.0f; }

public: // AABB用関数群

	/// <summary>
	/// 更新関数(AABB)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="size">AABBサイズ</param>
	virtual void Update(Vector3 center, Vector3 size) { 
		center; 
		size;
	};

	/// <summary>
	/// AABB最小点ゲッター
	/// </summary>
	/// <returns>AABB最小点</returns>
	virtual Vector3 GetMin() { return Vector3(0.0f, 0.0f, 0.0f); }
	/// <summary>
	/// AABB最大点ゲッター
	/// </summary>
	/// <returns>AABB最大点</returns>
	virtual Vector3 GetMax() { return Vector3(0.0f, 0.0f, 0.0f); }

public: // OBB用関数群

	/// <summary>
	/// 更新関数(OBB)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="rotate">回転</param>
	virtual void Update(Vector3 center, Vector3 size, Vector3 rotate) {
		size;
		rotate;
		center;
	};

	/// <summary>
	/// 座標軸ゲッター
	/// </summary>
	/// <returns>座標軸</returns>
	virtual Vector3* GetOtientatuons() { return nullptr; }

protected: // メンバ関数

	// コライダー形状
	ColliderShape colliderShape_;

};
