#pragma once
#include <vector>
#include "BaseDescriptorHeap.h"

// 1クラスの前方宣言
class SRV;

/// <summary>
/// 深度ステンシルビュークラス
/// </summary>
class DSV : public BaseDescriptorHeap
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">DirextX12のデバイス</param>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	void Init(ID3D12Device* device, int32_t width, int32_t height);

	/// <summary>
	/// 深度クリア関数
	/// </summary>
	/// <param name="index">クリアするインデックス</param>
	/// <param name="cmdList">コマンドリスト</param>
	void ClearDepth(UINT index, ID3D12GraphicsCommandList* cmdList);

public: // アクセッサ等

private: // プライベートなメンバ関数

	/// <summary>
	/// 前後関係保持用のリソース生成関数
	/// </summary>
	/// <param name="width">画面横幅</param>
	/// <param name="height">画面縦幅</param>
	void CreateDepthStencilResource(int32_t width, int32_t height);

private: // メンバ変数

	// 深度ステンシルビューリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> depthStencilResource_;

};

