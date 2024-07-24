#pragma once
#include "../IResource.h"
#include "../../../Math/Vector2.h"
#include "../../../../Externals/DirectXTex/DirectXTex.h"

/// <summary>
/// テクスチャリソースクラス
/// </summary>
class TextureResource : public IResource
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="heaps">ヒープマネージャー</param>
	/// <param name="filePath">ファイルパス</param>
	void Init(DirectXDevice* device, HeapManager* heaps, std::string filePath);

	/// <summary>
	/// テクスチャデータの転送をコマンドリストに命令する関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <returns>中間リソース</returns>
	void UploadTextureData(DirectXDevice* device);

public: // アクセッサ

	/// <summary>
	/// SRV上の登録インデックスゲッター
	/// </summary>
	/// <returns>SRV上の登録インデックス</returns>
	int GetSRVIndex() const { return info_.index_; }

	/// <summary>
	/// テクスチャ解像度ゲッター
	/// </summary>
	/// <returns>テクスチャ解像度</returns>
	Vector2 GetTextureSize() const { return { static_cast<float>(width_), static_cast<float>(height_) }; }

	/// <summary>
	/// GPU上のアドレスゲッター
	/// </summary>
	/// <returns>GPU上のアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureAddress() const { return info_.gpuView_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// テクスチャロード関数
	/// </summary>
	/// <param name="filePath">テクスチャまでのファイルパス</param>
	void Load(const std::string& filePath);

public: // パブリックなメンバ変数

	// テクスチャ解像度
	int width_;  // 横幅
	int height_; // 縦幅

	// リソース設定
	D3D12_RESOURCE_DESC desc_;
	// ヒープ設定
	D3D12_HEAP_PROPERTIES properties_;

private: // メンバ変数

	// ミップマップ付きテクスチャ
	DirectX::ScratchImage mipImages_;

	// SRV上の登録データ
	SRVInfo info_;

	// 中間リソースのポインタ
	ID3D12Resource* intermediateResource_ = nullptr;

};

