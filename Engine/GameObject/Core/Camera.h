#pragma once
#include "../IObject.h"
#include "../../Input/Input.h"
#include "../../Base/Buffer/BufferStructs.h"
#include "../../Base/Resource/Data/ConstantBuffer.h"
#include "../../Base/Resource/Rendering/RenderResource.h"
#include "../../Base/Resource/Rendering/DepthStencil.h"

// クラスの前方宣言
class RendererManager;

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
	/// レンダリング後のテクスチャゲッター
	/// </summary>
	/// <returns>レンダリング後のテクスチャ</returns>
	Texture GetRenderTexture();

	/// <summary>
	/// カメラデータまでのアドレスゲッター
	/// </summary>
	/// <returns>カメラデータまでのアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetBufferView() { return cameraDataBuffer_.GetGPUView(); }

	/// <summary>
	/// レンダリング予定リソースのゲッター
	/// </summary>
	/// <returns>レンダリング予定リソース</returns>
	RenderResource* GetRendererResource() { return &renderResource_; }

	/// <summary>
	/// DSVリソースゲッター
	/// </summary>
	/// <returns>深度マップ</returns>
	DepthStencil* GetDepthStencil() { return &depthStencil_; }

public: // その他関数

	/// <summary>
	///	このカメラを使用する際に呼び出す関数
	/// </summary>
	void UseThisCamera();

public: // パブリックなメンバ変数

	// 視野角
	float fov_ = 0.45f;

protected: // メンバ変数

	// 描画管理マネージャ
	RendererManager* rendererManager_ = nullptr;

	// 定数バッファデータ
	ConstantBuffer<CameraData> cameraDataBuffer_;

	// レンダリングを行うリソース
	RenderResource renderResource_;
	// レンダリング後の結果を保存するリソース
	RenderResource textureResource_;
	// DSVリソース
	DepthStencil depthStencil_;

	// 入力検知用
	Input* input_ = nullptr;

	// カメラ使用トリガー
	bool isUseThisCamera_ = false;

	// カメラのメッシュレット表示切り替えトリガー
	bool isDrawMeshlets_ = false;

	// ビュー行列
	Matrix4x4 viewMatrix_;
	// カメラ用ビュープロジェクション行列
	Matrix4x4 viewProjectionMatrix_;

};

