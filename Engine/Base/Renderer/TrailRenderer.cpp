#include "TrailRenderer.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Base/DirectXCommon.h"

void TrailRenderer::Init(DirectXDevice* device, DXC* dxc)
{
	// ルートシグネチャの生成
	rootSignature_.Init();
	rootSignature_.CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	rootSignature_.CreateCBVParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	rootSignature_.CreateCBVParameter(2, D3D12_SHADER_VISIBILITY_ALL);
	rootSignature_.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_ALL);
	rootSignature_.CreateDescriptorTableParameter(1, D3D12_SHADER_VISIBILITY_ALL);
	rootSignature_.CreateDescriptorTableParameter(2, D3D12_SHADER_VISIBILITY_ALL, D3D12_DESCRIPTOR_RANGE_TYPE_SRV, KEngine::Config::Rendering::kMaxTexture);
	rootSignature_.CreateSampler(
		0,
		D3D12_SHADER_VISIBILITY_PIXEL,
		D3D12_FILTER_MIN_MAG_MIP_LINEAR,
		D3D12_COMPARISON_FUNC_NEVER,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP);
	// デバイスをセットする
	rootSignature_.SetDevice(device->GetDevice());
	// ルートシグネチャを生成
	rootSignature_.CreateRootSignature();

	// PSOの生成
	pso_.Init(rootSignature_.GetRootSignature(), dxc, PSO::PSOType::Vertex)
		.SetRasterizerState(D3D12_CULL_MODE_NONE)
		.SetVertexShader("Engine/Resource/Shader/Trail/Trail.VS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/Trail/Trail.PS.hlsl")
		.Build(device->GetDevice());
}

void TrailRenderer::DrawCall(ID3D12GraphicsCommandList6* list)
{
	// ターゲット分ループする
	for (std::vector<Target>::iterator it = targets_.begin(); it != targets_.end(); it++) {
		// 描画先のRTVとDSVをセットする
		list->OMSetRenderTargets(1, &it->backBuffer_->rtvInfo_.cpuView_, false, &it->depthBuffer_->dsvInfo_.cpuView_);

		// リソースバリアをセットする
		D3D12_RESOURCE_STATES beforeBarrier = it->backBuffer_->GetBarrier();
		// リソースバリア変更
		it->backBuffer_->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);
		// 指定した深度で全画面クリア
		it->depthBuffer_->Clear(list);

		//ビューポートの設定
		D3D12_VIEWPORT viewport = {};
		// ウィンドウのクライアント領域のサイズと一致させて画面全体に表示
		viewport.Width = static_cast<float>(it->backBuffer_->width_);   // 画面横幅
		viewport.Height = static_cast<float>(it->backBuffer_->height_); // 画面縦幅
		viewport.TopLeftX = 0;											// 画面左上X座標
		viewport.TopLeftY = 0;											// 画面左上Y座標
		viewport.MinDepth = 0.0f;										// 最小深度
		viewport.MaxDepth = 1.0f;										// 最大深度
		// コマンドリストにビューポートを設定
		list->RSSetViewports(1, &viewport);

		// シザー矩形の設定
		D3D12_RECT scissorRect = {};
		// ビューポートと同一の矩形が表示
		scissorRect.left = 0;						   // 左
		scissorRect.right = it->backBuffer_->width_;   // 右
		scissorRect.top = 0;						   // 上
		scissorRect.bottom = it->backBuffer_->height_; // 下
		// コマンドリストにシザー矩形を設定
		list->RSSetScissorRects(1, &scissorRect);

		// テクスチャ用にSRVヒープの取得
		SRV* s = DirectXCommon::GetInstance()->GetSRV();

		// コマンドリストに通常描画ルートシグネチャを設定
		list->SetGraphicsRootSignature(rootSignature_.GetRootSignature());
		// コマンドリストに通常描画PSOを設定
		list->SetPipelineState(pso_.GetState());
		// 共通データのアドレスを渡す
		list->SetGraphicsRootConstantBufferView(0, it->view_);					// カメラデータ
		list->SetGraphicsRootDescriptorTable(5, s->GetFirstTexView());			// テクスチャデータ

		// 全ての描画命令を実行
		for (const std::function<void(ID3D12GraphicsCommandList6* cmdList)>& func : drawFunctions_) {
			func(list);
		}

		// 登録されている描画命令を全てクリアする
		drawFunctions_.clear();

		// バリアを元に戻す
		it->backBuffer_->ChangeResourceBarrier(beforeBarrier, list);
	}
}

void TrailRenderer::Reset()
{
	// ターゲット配列クリア
	targets_.clear();
}
