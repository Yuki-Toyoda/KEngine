#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"

/// <summary>
/// カメラ
/// </summary>
class MainCamera : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// ビュー行列ゲッター
	/// </summary>
	/// <returns>ビュー行列</returns>
	Matrix4x4 GetViewMatrix() { return viewMatrix_; }

	/// <summary>
	/// ビュープロジェクション行列ゲッター
	/// </summary>
	/// <returns>ビュープロジェクション行列</returns>
	Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; };

public: // パブリックなメンバ変数

	// 視野角
	float fov_;

private: // メンバ変数

	// 入力検知用
	Input* input_;

	// ビュー行列
	Matrix4x4 viewMatrix_;
	// カメラ用ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

};

