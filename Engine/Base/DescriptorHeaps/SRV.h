#pragma once
#include <vector>
#include "BaseDescriptorHeap.h"

/// <summary>
/// シェーダーリソースビュークラス
/// </summary>
class SRV : public BaseDescriptorHeap
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	void Initialize(ID3D12Device* device);

public: // アクセッサ等

	/// <summary>
	/// ヒープ使用数加算関数
	/// </summary>
	void AddUsedCount() { usedCount_++; }
	/// <summary>
	/// ヒープ使用数取得関数
	/// </summary>
	uint32_t GetUsedCount() { return usedCount_; }

private: // メンバ変数

	// ヒープの使用数カウント
	// 0番目はImGuiが使用しているため初期値は1となる
	uint32_t usedCount_ = 1;

};

