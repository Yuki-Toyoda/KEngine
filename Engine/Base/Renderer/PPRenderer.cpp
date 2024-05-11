#include "PPRenderer.h"

void PPRenderer::Init(DirectXDevice* device, ID3D12RootSignature* signature, DXC* dxc)
{
	// ルートシグネチャ取得
	//rootSignature_ = signature;

	// 通常描画用PSO初期化
	pso_.VertInit(signature, dxc)
		.SetVertexShader("Engine/Resource/Shader/PostProcess/PassThroughVS.hlsl")
		.SetVertPixelShader("Engine/Resource/Shader/PostProcess/PostProcessPS.hlsl")
		.VertBuild(device->GetDevice());
}

void PPRenderer::DrawCall(ID3D12GraphicsCommandList6* list)
{
	// textureの方に書き込む
	list->OMSetRenderTargets(1, &target_.texture->rtvInfo_.cpuView_, false, nullptr);
	// RootSignatureとPSOをセット
	//list->SetGraphicsRootSignature(rootSignature_);
	list->SetPipelineState(pso_.GetState());

	// データを登録
	//list->SetGraphicsRootConstantBufferView(0, );
	list->SetGraphicsRootDescriptorTable(1, target_.render->srvInfo_.gpuView_);
	//list->SetGraphicsRootDescriptorTable(2, it->depth->srvInfo.gpuView);

	// リソースバリアをセット
	D3D12_RESOURCE_STATES beforeBarrier = target_.texture->GetBarrier();
	target_.render->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list);	// 加工する画像
	target_.texture->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);	// 書き込み対象
	target_.depth->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list);	// 深度マップ
	// 全画面クリア
	target_.texture->Clear(list);

	// ビューポート
	D3D12_VIEWPORT viewport = {};
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = static_cast<float>(target_.texture->width_);
	viewport.Height = static_cast<float>(target_.texture->height_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	// viewportを設定
	list->RSSetViewports(1, &viewport);

	// シザー矩形
	D3D12_RECT scissorRect = {};
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = target_.texture->width_;
	scissorRect.top = 0;
	scissorRect.bottom = target_.texture->height_;
	// Scirssorを設定
	list->RSSetScissorRects(1, &scissorRect);

	// 描画する
	list->DrawInstanced(3, 1, 0, 0);

	// バリアを元に戻す
	target_.render->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list); // 加工する画像
	target_.texture->ChangeResourceBarrier(beforeBarrier, list);							 // 書き込み対象
	target_.depth->ChangeResourceBarrier(D3D12_RESOURCE_STATE_DEPTH_WRITE, list);			 // 深度マップ
}
