#pragma once
#include "BaseShape.h"

/// <summary>
/// コライダー形状(AABB)
/// </summary>
class AABB : public BaseShape {
public: // メンバ関数

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	~AABB() override {};

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="size">AABB半径</param>
	void Initialize(Vector3 center, Vector3 size) {
		colliderShape_ = ColliderShape::AABB; // 形状設定
		center_ = center; // 中心座標設定
		min_ = center_ - size; // 最小点設定
		max_ = center_ + size; // 最大点設定
	}

	/// <summary>
	/// 更新関数(AABB)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="size">AABBサイズ</param>
	void Update(Vector3 center, Vector3 size) override {
		center_ = center; // 中心座標設定
		min_ = center_ - size; // 最小点設定
		max_ = center_ + size; // 最大点設定
	};

	/// <summary>
	/// 中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	Vector3 GetCenter() override { return center_; }

	/// <summary>
	/// 最小点ゲッター
	/// </summary>
	/// <returns>最小点</returns>
	Vector3 GetMin() override { return min_; }
	/// <summary>
	/// 最大点ゲッター
	/// </summary>
	/// <returns>最大点</returns>
	Vector3 GetMax() override { return max_; }

private: // メンバ変数

	// 中心座標
	Vector3 center_;
	// 最小点
	Vector3 min_;
	// 最大点
	Vector3 max_;

};
