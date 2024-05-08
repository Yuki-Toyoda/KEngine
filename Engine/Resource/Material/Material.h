#pragma once
#include "../../GameObject/WorldTransform.h"
#include "../../Math/Vector4.h"
#include "../../Base/Resource/Data/ConstantBuffer.h"
#include "../../Resource/Texture/Texture.h"

/// <summary>
/// マテリアルクラス
/// </summary>
class Material final
{
public: // サブクラス

	/// <summary>
	/// マテリアルデータ構造体
	/// </summary>
	struct MaterialData {
		Vector4 color;
	};


public: // メンバ関数

	// コンストラクタ
	Material();
	// デストラクタ
	~Material() = default;

	/// <summary>
	/// マテリアルのアップロード
	/// </summary>
	void UploadMaterial();

public: // アクセッサ等

	/// <summary>
	/// マテリアルのバッファアドレスゲッター
	/// </summary>
	/// <returns>マテリアルのバッファアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetBufferAddress() { return materialBuffer_.get()->GetGPUView(); }

	/// <summary>
	/// テクスチャのアドレスゲッター
	/// </summary>
	/// <returns>テクスチャまでのアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetTexAddress() { return tex_.GetView_(); }

public: // パブリックなメンバ変数

	// マテリアル用バッファ
	std::unique_ptr<ConstantBuffer<MaterialData>> materialBuffer_;

	// テクスチャ
	Texture tex_;

	// マテリアル色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// uvトランスフォーム
	WorldTransform uvTransform_;

	// ライティングを行うか
	bool enableLighting_ = false;

};

