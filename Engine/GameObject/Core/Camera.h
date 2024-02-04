#pragma once
#include "../BaseObject.h"
#include "../../Input/Input.h"

/// <summary>
/// カメラ
/// </summary>
class Camera : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

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
	/// カメラの使用トリガーセッター
	/// </summary>
	/// <param name="isUsed">使用するか</param>
	void SetIsUseThisCamera(bool isUsed) { isUseThisCamera_ = isUsed; }
	/// <summary>
	/// カメラの使用トリガーゲッター
	/// </summary>
	/// <returns>カメラが使用中か</returns>
	bool GetIsUsedCamera() { return isUseThisCamera_; }

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

public: // その他関数

	/// <summary>
	///	このカメラを使用する際に呼び出す関数
	/// </summary>
	void UseThisCamera();

private: // メンバ関数

	/// <summary>
	///	ViewProjectionMatrixの書き込み先の指定関数
	/// </summary>
	void SetVPDataTarget();

public: // パブリックなメンバ変数

	// 視野角
	float fov_;

protected: // メンバ変数

	// 入力検知用
	Input* input_;

	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// カメラ使用トリガー
	bool isUseThisCamera_;

	// ビュー行列
	Matrix4x4 viewMatrix_;
	// カメラ用ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

	// ビュープロジェクション行列をコマンドマネージャーに送るためのポインタ
	Matrix4x4* vpDataTarget_ = nullptr;

};

