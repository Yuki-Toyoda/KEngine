#pragma once
#include <vector>
#include "IDescriptorHeap.h"

/// <summary>
/// シェーダーリソースビュークラス
/// </summary>
class SRV : public IDescriptorHeap
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	void Init(ID3D12Device* device);

public: // アクセッサ等

	/// <summary>
	/// ヒープ使用数取得関数
	/// </summary>
	uint32_t GetUsedCount() { return usedCount_.GetUsedCount(); }

	/// <summary>
	/// 使用されていないヒープのインデックスを取得する
	/// </summary>
	/// <returns>インデックス</returns>
	Index UseEmpty() { return usedCount_.UseEmpty(); }

private: // メンバ変数

	// ヒープ使用最大数
	const int kMaxSize_ = 128;

	// ヒープの使用数カウント
	// 0番目はImGuiが使用しているため初期値は1となる
	IndexList usedCount_{ kMaxSize_ };

	// ImGui用Index
	Index imGuiIndex_;

};

