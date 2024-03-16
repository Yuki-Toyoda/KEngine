#pragma once
#include "../IPrimitive.h"

// クラスの前方宣言
class Camera;

/// <summary>
/// ビルボードを行う平面
/// </summary>
class BillboardPlane : public IPrimitive
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

	/// <summary>
	/// ビルボード行列の更新
	/// </summary>
	void UpdateBillboardMat();

	/// <summary>
	/// メインカメラのセッター
	/// </summary>
	/// <param name="camera">メインカメラ本体</param>
	void SetMainCamera(Camera* camera) { camera_ = camera; }

private: // メンバ変数

	// 使用中カメラ
	Camera* camera_ = nullptr;

};

