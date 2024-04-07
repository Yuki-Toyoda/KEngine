#include "IDescriptorHeap.h"

D3D12_CPU_DESCRIPTOR_HANDLE IDescriptorHeap::GetCPUHandle(uint32_t index)
{
	// ヒープの中から最初のCPUハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = heap_->GetCPUDescriptorHandleForHeapStart();
	// 取得したハンドルから指定したオフセット分ずらす
	handleCPU.ptr += (kDescriptorSize_ * index);
	// ハンドルを返す
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE IDescriptorHeap::GetGPUHandle(uint32_t index)
{
	// ヒープの中から最初のCPUハンドルを取得
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = heap_->GetGPUDescriptorHandleForHeapStart();
	// 取得したハンドルから指定したオフセット分ずらす
	handleGPU.ptr += (kDescriptorSize_ * index);
	// ハンドルを返す
	return handleGPU;
}

ID3D12DescriptorHeap* IDescriptorHeap::CreateDescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE heapType, UINT numDescriptors, bool shaderVisible)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	ID3D12DescriptorHeap* descriptorHeap = nullptr; // インスタンス生成
	D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc{}; // 設定
	descriptorHeapDesc.Type = heapType; // 種類設定
	descriptorHeapDesc.NumDescriptors = numDescriptors; // ヒープ番号取得
	// フラッグの設定取得
	descriptorHeapDesc.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;

	// 設定を基にヒープを生成
	result = device_->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	// 生成確認
	assert(SUCCEEDED(result));

	// 生成したヒープを返す
	return descriptorHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE IDescriptorHeap::GetDescriptorInclementSize(const D3D12_CPU_DESCRIPTOR_HANDLE& other, int offsetInDescriptors, UINT descriptorIncrementSize) const
{
	// ディスクリプタハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE handle;
	// オフセットが0以下であれば最初のハンドルを返す
	if (offsetInDescriptors <= 0)
		handle = other;
	else // それ以外の場合は増加分を取得しそれを返す
		handle.ptr = other.ptr + (offsetInDescriptors * descriptorIncrementSize);

	// 取得したハンドルを返す
	return handle;
}
