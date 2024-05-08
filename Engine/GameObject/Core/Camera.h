#pragma once
#include "../IObject.h"
#include "../../Input/Input.h"
#include "../../Base/Buffer/BufferStructs.h"
#include "../../Base/Resource/Data/ConstantBuffer.h"

/// <summary>
/// カメラ
/// </summary>
class Camera : public IObject
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

	/// <summary>
	/// カメラデータまでのアドレスゲッター
	/// </summary>
	/// <returns>カメラデータまでのアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetBufferView() { return cameraDataBuffer_.GetGPUView(); }

public: // その他関数

	/// <summary>
	///	このカメラを使用する際に呼び出す関数
	/// </summary>
	void UseThisCamera();

public: // パブリックなメンバ変数

	// 視野角
	float fov_;

protected: // メンバ変数

	// 定数バッファデータ
	ConstantBuffer<CameraData> cameraDataBuffer_;

	// 入力検知用
	Input* input_;

	// カメラ使用トリガー
	bool isUseThisCamera_;

	// カメラのメッシュレット表示切り替えトリガー
	bool isDrawMeshlets_ = false;

	// ビュー行列
	Matrix4x4 viewMatrix_;
	// カメラ用ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

};

