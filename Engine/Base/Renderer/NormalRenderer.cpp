#include "NormalRenderer.h"
#include "../../../Externals/imgui/ImGuiManager.h"

void NormalRenderer::Init(DirectXDevice* device, ID3D12RootSignature* signature, DXC* dxc)
{
	// 通常描画用PSO初期化
	pso_.Init(signature, dxc)
		.SetMeshShader("Engine/Resource/Shader/MeshletMS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/MeshletPS.hlsl")
		.Build(device->GetDevice());
}

void NormalRenderer::DrawCall(ID3D12GraphicsCommandList6* list)
{
	// ImGuiの受付終了
	ImGui::EndFrame();

	// 描画先のRTVとDSVをセットする
	list->OMSetRenderTargets(1, &backBuffer_->rtvInfo_.cpuView_, false, &depthBuffer_->dsvInfo_.cpuView_);

	// リソースバリアをセットする
	D3D12_RESOURCE_STATES beforeBarrier = backBuffer_->GetBarrier();
	// リソースバリア変更
	backBuffer_->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);
	// 全画面クリア
	backBuffer_->Clear(list);
	// 指定した深度で全画面クリア
	depthBuffer_->Clear(list);

	//ビューポートの設定
	D3D12_VIEWPORT viewport = {};
	// ウィンドウのクライアント領域のサイズと一致させて画面全体に表示
	viewport.Width	  = static_cast<float>(backBuffer_->width_);  // 画面横幅
	viewport.Height   = static_cast<float>(backBuffer_->height_); // 画面縦幅
	viewport.TopLeftX = 0;										  // 画面左上X座標
	viewport.TopLeftY = 0;										  // 画面左上Y座標
	viewport.MinDepth = 0.0f;									  // 最小深度
	viewport.MaxDepth = 1.0f;									  // 最大深度
	// コマンドリストにビューポートを設定
	list->RSSetViewports(1, &viewport);

	// シザー矩形の設定
	D3D12_RECT scissorRect = {};
	// ビューポートと同一の矩形が表示
	scissorRect.left   = 0;					   // 左
	scissorRect.right  = backBuffer_->width_;  // 右
	scissorRect.top    = 0;					   // 上
	scissorRect.bottom = backBuffer_->height_; // 下
	// コマンドリストにシザー矩形を設定
	list->RSSetScissorRects(1, &scissorRect);

	// コマンドリストにPSOを設定
	list->SetPipelineState(pso_.GetState());

	// ImGuiを描画
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list);

	// バリアを元に戻す
	backBuffer_->ChangeResourceBarrier(beforeBarrier, list);
}
