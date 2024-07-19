#pragma once
#include "IRenderingResource.h"

/// <summary>
/// 深度ステンシルリソースクラス
/// </summary>
class DepthStencil : public IRenderingResource
{
public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DepthStencil() = default;

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

public: // パブリックなメンバ変数

	// DSVの登録情報
	DSVInfo dsvInfo_;

	// SRVの登録情報
	SRVInfo srvInfo_;

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};

