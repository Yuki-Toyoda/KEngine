#pragma once
#include"../../Core/Camera.h"
#include "../../../Input/Input.h"
class InGameCamera: public Camera
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

	void Shake();
	bool GetIsShake() { return isShake_; }
//public: // アクセッサ等
//
//	/// <summary>
//	/// カメラの使用トリガーセッター
//	/// </summary>
//	/// <param name="isUsed">使用するか</param>
//	void SetIsUseThisCamera(bool isUsed) { isUseThisCamera_ = isUsed; }
//	/// <summary>
//	/// カメラの使用トリガーゲッター
//	/// </summary>
//	/// <returns>カメラが使用中か</returns>
//	bool GetIsUsedCamera() { return isUseThisCamera_; }
//
//	/// <summary>
//	/// ビュー行列ゲッター
//	/// </summary>
//	/// <returns>ビュー行列</returns>
//	Matrix4x4 GetViewMatrix() { return viewMatrix_; }
//
//	/// <summary>
//	/// ビュープロジェクション行列ゲッター
//	/// </summary>
//	/// <returns>ビュープロジェクション行列</returns>
//	Matrix4x4 GetViewProjectionMatrix() { return viewProjectionMatrix_; };
//
//public: // その他関数
//
//	/// <summary>
//	///	このカメラを使用する際に呼び出す関数
//	/// </summary>
//	void UseThisCamera();
//
private: // メンバ関数
	//シェイクする時間
	int shakeCount_;
	int maxShakeCount_=30;
	//画面シェイクの強さ
	float shakeForce_;
	float kShakeForce_=4.0f;
	//前フレームの座標
	Vector3 translate_;
	//シェイクしているか
	bool isShake_;
};
