#include "RendererManager.h"

void RendererManager::Init(DirectXDevice* device, SRV* srv)
{
	// 引数からSRVを取得
	srv_ = srv;

	// コマンド管理クラスの初期化
	command_.Init(device);

	// 通常描画レンダラーを初期化
	normalRenderer_.Init(device, RootSignatureManager::GetInstance()->GetRootSignature(0), &dxc_);
}

void RendererManager::DrawCall()
{
	// リストをポインタ化して取得
	ID3D12GraphicsCommandList6* list = command_.List();

	// コマンドリストにSRVのDescriptorHeapを設定する
	ID3D12DescriptorHeap* descriptorHeaps[] = { srv_->GetDescriptorHeap() };
	list->SetDescriptorHeaps(1, descriptorHeaps);

	// 通常描画を行う
	normalRenderer_.DrawCall(list);

	// コマンドリストの実行
	command_.Execute();
}
