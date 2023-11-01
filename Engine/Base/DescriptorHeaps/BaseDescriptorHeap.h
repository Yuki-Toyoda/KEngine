#pragma once
#include <stdint.h>
#include <cassert>
#include "../Device/DirectXDevice.h"

/// <summary>
/// ディスクリプタヒープの基底クラス
/// </summary>
class BaseDescriptorHeap
{
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
	uint32_t GetDescriptorSize() { return size_; }

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
	uint32_t kDescriptorSize_;
	// ヒープのサイズ
	uint32_t size_;
};

