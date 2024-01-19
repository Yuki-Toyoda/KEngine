#include "ParticleCommand.h"
#include "../WinApp.h"
#include "../DescriptorHeaps/RTV.h"
#include "../DescriptorHeaps/SRV.h"
#include "../DescriptorHeaps/DSV.h"

void ParticleCommand::PreDraw(ID3D12GraphicsCommandList* cmdList)
{
	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier = SettingResourceBarrier(
		rtv_->GetBackBuffer(),
		D3D12_RESOURCE_STATE_PRESENT,	   // 画面に表示
		D3D12_RESOURCE_STATE_RENDER_TARGET // 書き込めるように
	);

	// リソースバリアセット
	cmdList->ResourceBarrier(1, &barrier);

	// 書き込み予定バッファのハンドルを取得
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtv_->GetCPUHandle(rtv_->GetBackBufferIndex());
	// 深度ステンシルビューのハンドルも取得
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsv_->GetCPUHandle(0);
	// 取得したハンドルを基に描画先のRTVとDSVの設定
	cmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

	// 指定した深度で全画面のクリア
	//dsv_->ClearDepth(0, cmdList);

	// 描画に用いるSRVのディスクリプタヒープの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srv_->GetDescriptorHeap() }; // SRVからヒープを取得
	cmdList->SetDescriptorHeaps(1, descriptorHeaps);						 // 取得したヒープをコマンドリストにセット

	// インスタンス取得
	WinApp* winApp = WinApp::GetInstance();

	// ビューポートの設定
	D3D12_VIEWPORT viewPort = {}; // 空のインスタンス生成
	// クライアント領域のサイズを取得し画面全体に表示させる
	viewPort.Width = static_cast<float>(winApp->kWindowWidth);	// ビューポートの横幅設定
	viewPort.Height = static_cast<float>(winApp->kwindowHeight); // ビューポートの縦幅設定
	viewPort.TopLeftX = 0.0f;													// ビューポートの左上開始地点x座標
	viewPort.TopLeftY = 0.0f;													// ビューポートの左上開始地点y座標
	viewPort.MinDepth = 0.0f;													// ビューポートの最低深度設定
	viewPort.MaxDepth = 1.0f;													// ビューポートの最高深度設定
	cmdList->RSSetViewports(1, &viewPort);										// 設定したビューポートをコマンドリストを設定

	// シザー矩形の設定
	D3D12_RECT scissorRect = {}; // 空のインスタンス生成
	// ビューポートと同じように構成
	scissorRect.left = 0;
	scissorRect.right = winApp->kWindowWidth;
	scissorRect.top = 0;
	scissorRect.bottom = winApp->kwindowHeight;
	cmdList->RSSetScissorRects(1, &scissorRect); // コマンドリストに生成したシザー矩形をセット
}

void ParticleCommand::PostDraw(ID3D12GraphicsCommandList* cmdList)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// TransitionBarrierの設定
	D3D12_RESOURCE_BARRIER barrier = SettingResourceBarrier(
		rtv_->GetBackBuffer(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, // 書き込めるように
		D3D12_RESOURCE_STATE_PRESENT	   // 画面に表示	
	);

	// リソースバリアセット
	cmdList->ResourceBarrier(1, &barrier);

	// コマンドリストの内容を確定、命令をこれ以上受け付けない
	result = cmdList->Close(); // クローズ
	assert(SUCCEEDED(result)); // 確定確認
}

void ParticleCommand::CreatePSO(ID3D12Device* device, DXC* dxc, ID3D12RootSignature* signature, std::wstring vs, std::wstring ps, int32_t blendType, UINT wire)
{
	std::unique_ptr<PSO> newPSO = std::make_unique<PSO>();		  // インスタンスを生成
	newPSO->Init(device, signature, dxc, vs, ps, blendType, false, wire); // 初期化

	// PSO配列に追加
	pso_ = (std::move(newPSO));
}
