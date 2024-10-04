#pragma once
#include <stdint.h>
#include <cassert>
#include "Engine/Base/Device/DirectXDevice.h"
#include "Engine/Utility/IndexList/IndexList.h"

/// <summary>
/// ディスクリプタヒープの基底クラス
/// </summary>
class IDescriptorHeap
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IDescriptorHeap() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="heapType">ヒープの種類</param>
	/// <param name="size">ヒープのサイズ</param>
	IDescriptorHeap(ID3D12Device2* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t size);

	/// <summary>
	/// コンストラクタ(SRV用)
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="heapType"></param>
	/// <param name="descSize"></param>
	/// <param name="indexSize"></param>
	IDescriptorHeap(ID3D12Device2* device, D3D12_DESCRIPTOR_HEAP_TYPE heapType, uint32_t descSize, uint32_t indexSize);

public: // アクセッサ等

	/// <summary>
	/// CPU上のディスクリプタハンドルゲッター
	/// </summary>
	/// <param name="index">取得するインデックス</param>
	/// <returns>指定したインデックスのディスクリプタハンドル</returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUHandle(uint32_t index);
	/// <summary>
	/// GPU上のディスクリプタハンドルゲッター
	/// </summary>
	/// <param name="index">取得するインデックス</param>
	/// <returns>指定したインデックスのディスクリプタハンドル</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUHandle(uint32_t index);

	/// <summary>
	/// ヒープゲッター
	/// </summary>
	/// <returns>ヒープ</returns>
	ID3D12DescriptorHeap* GetDescriptorHeap() { return heap_.Get(); }
	/// <summary>
	/// ヒープサイズのゲッター
	/// </summary>
	/// <returns>ヒープサイズ</returns>
	uint32_t GetDescriptorSize() { return kSize_; }

protected: // 継承メンバ関数

	/// <summary>
	/// ディスクリプタヒープ作成関数
	/// </summary>
	/// <param name="heapType">作成するヒープの種類</param>
	/// <param name="numDescriptors"></param>
	/// <param name="shaderVisible"></param>
	/// <returns></returns>
	ID3D12DescriptorHeap* CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDescriptors, bool shaderVisible);

	/// <summary>
	/// ディスクリプタの実際のサイズゲッター
	/// </summary>
	/// <param name="other">取得を開始するハンドル</param>
	/// <param name="offsetInDescriptors">そのディスクリプタからのオフセット</param>
	/// <param name="descriptorIncrementSize">そこからの増加分</param>
	/// <returns></returns>
	D3D12_CPU_DESCRIPTOR_HANDLE GetDescriptorInclementSize(
		const D3D12_CPU_DESCRIPTOR_HANDLE& other, 
		int offsetInDescriptors, UINT descriptorIncrementSize) const;

protected: // 継承メンバ変数

	// デバイスの実体
	ID3D12Device* device_ = nullptr;

	// ヒープ本体
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> heap_;

	// ディスクリプタ1つ分のサイズ定数
	const uint32_t kDescriptorSize_;
	// ヒープのサイズ
	const uint32_t kSize_;

	// 使用済みインデックス管理用
	IndexList indexList_;

};

/// <summary>
/// ヒープ情報構造体用基底クラス
/// </summary>
struct HeapInfo {

	// ヒープ上のインデックス格納用
	Index index_;
	// CPU上のアドレス
	D3D12_CPU_DESCRIPTOR_HANDLE cpuView_{};

	// デフォルトコンストラクタ
	HeapInfo() = default;

	/// <summary>
	/// アドレスセッター
	/// </summary>
	/// <param name="heap">ヒープ本体</param>
	virtual void SetView(IDescriptorHeap* heap) {
		cpuView_ = heap->GetCPUHandle(index_);
	}

	/// <summary>
	/// ムーブコンストラクタ
	/// </summary>
	/// <param name="other">代入するヒープ情報</param>
	HeapInfo(HeapInfo&& other) noexcept {
		// 引数から情報取得
		index_ = std::exchange(other.index_, Index()); // インデックス
		cpuView_ = std::exchange(other.cpuView_, {});  // アドレス
	}

	/// <summary>
	/// 代入演算子オーバーロード
	/// </summary>
	/// <param name="other">代入するヒープ情報</param>
	/// <returns>代入後のデータ</returns>
	HeapInfo& operator=(HeapInfo&& other) noexcept {
		// 代入先と一致していないときのみ代入を行う
		if (this != &other) {
			// 引数から情報取得
			index_ = std::exchange(other.index_, Index()); // インデックス
			cpuView_ = std::exchange(other.cpuView_, {});  // アドレス
		}
		// データを返す
		return *this;
	}

	// IndexListのバグを防ぐためコピー操作を禁止する
	HeapInfo(const HeapInfo&) = delete;
	HeapInfo& operator=(const HeapInfo&) = delete;
};

