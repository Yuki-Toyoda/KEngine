#pragma once
#include "IRenderingResource.h"

/// <summary>
/// バックバッファリソースクラス
/// </summary>
class BackBuffer : public IRenderingResource
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	BackBuffer() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="swapchain">スワップチューン</param>
	/// <param name="index">スワップチューン上のインデックス</param>
	BackBuffer(IDXGISwapChain4* swapchain, int index);

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

public: // パブリックなメンバ変数

	// RTVの登録情報
	RTVInfo rtvInfo_;

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};

