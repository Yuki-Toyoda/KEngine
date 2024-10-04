#pragma once
#include "BaseShape.h"
#include "Engine/Math/Matrix4x4.h"

/// <summary>
/// コライダー形状(OBB)
/// </summary>
class OBB : public BaseShape {
public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~OBB() override {};

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	void Init(Vector3* scale, Vector3* rotate, Vector3* translate) {
		colliderShape_ = ColliderShape::OBB; // 形状設定
		scale_ = scale;			// 大きさ
		rotate_ = rotate;		// 回転
		translate_ = translate; // 位置座標
	}

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override {
		ImGui::DragFloat3("scale", &scale_->x);
		ImGui::DragFloat3("rotate", &rotate_->x);
		ImGui::DragFloat3("tranlate", &translate_->x);

		int count = 0;
		for (Vector3 v : GetOtientatuons()) {
			std::string name = "Otientatuons" + count;
			ImGui::DragFloat3(name.c_str(), &v.x);
			count++;
		}
	};

	/// <summary>
	/// 中心座標のゲッター
	/// </summary>
	/// <returns>中心座標</returns>
	const Vector3 GetCenter() override { return *translate_; }

	/// <summary>
	/// 座標軸ゲッター
	/// </summary>
	/// <returns>座標軸</returns>
	const std::vector<Vector3> GetOtientatuons() override { 
		// 回転行列を求める
		Matrix4x4 rMat = Matrix4x4::MakeRotate(*rotate_);

		// 回転軸を求める
		Vector3 o1 = { rMat.m[0][0], rMat.m[0][1], rMat.m[0][2] };
		Vector3 o2 = { rMat.m[1][0], rMat.m[1][1], rMat.m[1][2] };
		Vector3 o3 = { rMat.m[2][0], rMat.m[2][1], rMat.m[2][2] };

		// 各列の正規化を行う
		o1.Normalize();
		o2.Normalize();
		o3.Normalize();

		// 回転軸格納用
		std::vector<Vector3> o;
		o.push_back(o1);
		o.push_back(o2);
		o.push_back(o3);

		// 回転軸を求める
		return o;
	}

	/// <summary>
	/// 大きさゲッター
	/// </summary>
	/// <returns></returns>
	const Vector3 GetSize() override { return *scale_; }

	/// <summary>
	/// 回転角ゲッター
	/// </summary>
	/// <returns>回転角</returns>
	const Vector3 GetRotate() { return *rotate_; }

private: // メンバ変数

	// 各種情報
	Vector3* scale_;	 // 大きさ
	Vector3* rotate_;	 // 回転
	Vector3* translate_; // 座標

};
