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
	void Init(Vector3* center, float* radius) {
		colliderShape_ = ColliderShape::Sphere; // 形状設定
		center_ = center; // 中心座標設定
		radius_ = radius; // 半径設定
	};

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override {
		ImGui::DragFloat3("Center", &center_->x);
		ImGui::DragFloat("Radius", radius_);
	};

	/// <summary>
	/// 中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	const Vector3 GetCenter() override { return *center_; }

	/// <summary>
	/// 球の当たり判定半径ゲッター
	/// </summary>
	/// <returns>球の当たり判定半径</returns>
	const float GetRadius() override { return *radius_; }

private: // メンバ変数

	// 中心座標
	Vector3* center_;
	// 当たり判定半径
	float* radius_;

};

