#pragma once
#include "BaseShape.h"

/// <summary>
/// コライダー形状(球)
/// </summary>
class Sphere : public BaseShape {
public: // メンバ関数

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	~Sphere() override {};

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="radius">当たり判定半径</param>
	void Initialize(Vector3 center, float radius) {
		colliderShape_ = ColliderShape::Sphere; // 形状設定
		center_ = center; // 中心座標設定
		radius_ = radius; // 半径設定
	};

	/// <summary>
	/// 更新関数(球)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="radius">半径</param>
	void Update(Vector3 center, float radius) override{
		center_ = center; // 中心座標設定
		radius_ = radius; // 半径設定
	};

	/// <summary>
	/// 中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	Vector3 GetCenter() override { return center_; }

	/// <summary>
	/// 球の当たり判定半径ゲッター
	/// </summary>
	/// <returns>球の当たり判定半径</returns>
	float GetRadius() override { return radius_; }

private: // メンバ変数

	// 中心座標
	Vector3 center_;
	// 当たり判定半径
	float radius_;

};
