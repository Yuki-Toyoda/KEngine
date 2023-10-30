#include "BaseDescriptorHeap.h"

D3D12_CPU_DESCRIPTOR_HANDLE BaseDescriptorHeap::GetCPUHandle(uint32_t index)
{
	// ヒープの中から最初のCPUハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = heap_->GetCPUDescriptorHandleForHeapStart();
	// 取得したハンドルから指定したオフセット分ずらす
	handleCPU.ptr += (size_ * index);
	// ハンドルを返す
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE BaseDescriptorHeap::GetGPUHandle(uint32_t index)
{
	// ヒープの中から最初のCPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = heap_->GetGPUDescriptorHandleForHeapStart();
	// 取得したハンドルから指定したオフセット分ずらす
	handleGPU.ptr += (size_ * index);
	// ハンドルを返す
	return handleGPU;
}

ID3D12DescriptorHeap* BaseDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	ID3D12DescriptorHeap* descriptorHeap = nullptr; // インスタンス生成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{}; // 設定
	descriptorHeapDesc.Type = heapType; // 種類設定
	descriptorHeapDesc.NumDescriptors = numDescriptors; // ヒープ番号取得
	descriptorHeapDesc.Flags = 
}
