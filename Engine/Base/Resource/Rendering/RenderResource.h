#pragma once
#include "BackBuffer.h"
#include "../../../Math/Vector2.h"

// クラスの前方宣言
class Texture;

/// <summary>
/// レンダリング用リソースクラス
/// </summary>
class RenderResource : public BackBuffer
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="heaps">ヒープマネージャー</param>
	void Init(DirectXDevice* device, HeapManager* heaps) override;

	/// <summary>
	/// 画面クリア関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	void Clear(ID3D12GraphicsCommandList6* list) override;

public: // アクセッサ

	/// <summary>
	/// SRV上の登録インデックス番号ゲッター
	/// </summary>
	/// <returns>SRV上の登録インデックス</returns>
	int GetSRVIndex() const { return srvInfo_.index_; }

	/// <summary>
	/// 解像度を返す関数
	/// </summary>
	/// <returns></returns>
	Vector2 GetTextureSize() const { return { static_cast<float>(width_), static_cast<float>(height_) }; }

	/// <summary>
	/// GPU上のアドレスゲッター
	/// </summary>
	/// <returns>GPU上のアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureAddress() const { return srvInfo_.gpuView_; }

public: // パブリックなメンバ変数

	// SRV上の登録データ
	SRVInfo srvInfo_;

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};

