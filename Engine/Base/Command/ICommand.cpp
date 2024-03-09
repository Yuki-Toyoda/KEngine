#include "ICommand.h"
#include "../DescriptorHeaps/SRV.h"

void ICommand::Init(ID3D12Device2* device, DXC* dxc, ID3D12RootSignature* signature, int32_t blendType)
{
	// PSOの生成
	CreatePSO(device, dxc, signature, blendType);
}

void ICommand::SetDescriptorHeap(RTV* rtv, SRV* srv, DSV* dsv)
{
	// メンバ変数にヒープ本体を代入
	rtv_ = rtv; // レンダーターゲットビュー
	srv_ = srv; // シェーダーリソースビュー
	dsv_ = dsv; // 深度ステンシルビュー
}

void ICommand::CreatePSO(ID3D12Device2* device, DXC* dxc, ID3D12RootSignature* signature, int32_t blendType)
{
	// 基底クラスでは記述を行わない
	// 以下は警告回避の参照
	device;
	dxc;
	signature;
	blendType;
}

D3D12_RESOURCE_BARRIER ICommand::SettingResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter)
{
	// TransitionBarrierの設定を行う
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION; // TransitionBarrier
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;	   // None
	barrier.Transition.pResource = resource;			   // 設定を行うバッファ
	barrier.Transition.StateBefore = stateBefore;		   // 現在のフロントバッファ
	barrier.Transition.StateAfter = stateAfter;			   // 現在のバックバッファ

	// 設定を返す
	return barrier;
}
