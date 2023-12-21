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
	void Init(Vector3* center, Vector3* size) {
		colliderShape_ = ColliderShape::AABB; // 形状設定
		center_ = center; // 中心座標設定
		size_ = size;	  // 大きさ設定
	}

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override {
		ImGui::DragFloat3("Center", &center_->x);
		ImGui::DragFloat3("Size", &size_->x);
	};

	/// <summary>
	/// 中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	const Vector3 GetCenter() override { return *center_; }

	/// <summary>
	/// 最小点ゲッター
	/// </summary>
	/// <returns>最小点</returns>
	const Vector3 GetMin() override { return (*center_ - *size_); }
	/// <summary>
	/// 最大点ゲッター
	/// </summary>
	/// <returns>最大点</returns>
	const Vector3 GetMax() override { return (*center_ + *size_); }

private: // メンバ変数

	// 中心座標
	Vector3* center_;
	// 大きさ
	Vector3* size_;

};
