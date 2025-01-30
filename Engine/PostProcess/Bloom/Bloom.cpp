#include "Bloom.h"
#include "Engine/Base/DirectXCommon.h"

void Bloom::Init()
{
	// 一時データ初期化用に各種データを取得する
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();								// DirectXCommon
	DirectXDevice* device	= dxCommon->GetDirectXDevice();								// デバイス
	HeapManager* heaps		= dxCommon->GetHeaps();										// ヒープマネージャー
	DXC* dxc				= dxCommon->GetInstance()->GetRendererManager()->GetDXC();	// DXC

	// 一時データ初期化
	brightnessData_.rResource.Init(device, heaps);	// 輝度抽出
	gaussX.rResource.Init(device, heaps);			// X軸ガウシアンブラー

	/// 輝度抽出用の設定
	// シグネチャ設定
	brightnessData_.signature.CreateCBVParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);
	brightnessData_.signature.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);
	brightnessData_.signature.CreateSampler(0);
	brightnessData_.signature.SetDevice(device->GetDevice());
	brightnessData_.signature.CreateRootSignature();
	// PSOの設定
	brightnessData_.pso.Init(brightnessData_.signature.GetRootSignature(), dxc, PSO::PSOType::Vertex)
		.SetDepthStencilState(false, true)
		.SetVertexShader("Engine/Resource/Shader/PostProcess/PassThroughVS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/PostProcess/Bloom/BrightnessFilter.PS.hlsl")
		.Build(device->GetDevice());

	/// X軸ガウシアンブラーの設定
	// シグネチャ設定
	gaussX.signature.CreateDescriptorTableParameter(0, D3D12_SHADER_VISIBILITY_PIXEL);
	gaussX.signature.CreateSampler(0);
	gaussX.signature.SetDevice(device->GetDevice());
	gaussX.signature.CreateRootSignature();
	// PSOの設定
	gaussX.pso.Init(gaussX.signature.GetRootSignature(), dxc, PSO::PSOType::Vertex)
		.SetDepthStencilState(false, true)
		.SetVertexShader("Engine/Resource/Shader/PostProcess/PassThroughVS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/PostProcess/Bloom/GaussianBlurX.hlsl")
		.Build(device->GetDevice());

	/// Y軸ガウシアンブラーの設定
	// PSOの設定
	gaussY.Init(gaussX.signature.GetRootSignature(), dxc, PSO::PSOType::Vertex)
		.SetDepthStencilState(false, true)
		.SetBlendState(1)
		.SetVertexShader("Engine/Resource/Shader/PostProcess/PassThroughVS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/PostProcess/Bloom/GaussianBlurY.hlsl")
		.Build(device->GetDevice());

	// 閾値リセット
	threshold_ = 0.95f;

	// バッファ初期化
	buffer_.Init(device);
}

void Bloom::Update()
{
	// バッファにデータを書き込み
	*buffer_.data_ = threshold_;
}

void Bloom::WriteBinding(std::ofstream* stream, IRootSignature* root, int* i)
{
	// 処理不要のため何も記述しない
	// 以下は警告回避用の記述
	stream;
	root;
	i;
}

void Bloom::WriteProcess(std::ofstream* stream)
{
	// 処理不要のため何も記述しない
	// 以下は警告回避用の記述
	stream;
}

void Bloom::SetToCommandList(ID3D12GraphicsCommandList6* cmdList, int* offset)
{
	// 処理不要のため何も記述しない
	// 以下は警告回避用の記述
	cmdList;
	offset;
}

void Bloom::PreCommand(ID3D12GraphicsCommandList6* list, RenderResource* target)
{
	// ビューポート
	D3D12_VIEWPORT viewport = {};
	// クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = static_cast<float>(target->width_);
	viewport.Height = static_cast<float>(target->height_);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	// シザー矩形
	D3D12_RECT scissorRect = {};
	// 基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = target->width_;
	scissorRect.top = 0;
	scissorRect.bottom = target->height_;

	/// Bloomは三段階で処理が行われる
	// ・輝度の抽出
	// ・X軸方向にガウシアンブラー
	// ・Y軸方向にガウシアンブラー

	/// 輝度の抽出処理
	// コマンドリストに情報をセット
	list->OMSetRenderTargets(1, &brightnessData_.rResource.rtvInfo_.cpuView_, false, nullptr);	// レンダー対象セット
	list->SetGraphicsRootSignature(brightnessData_.signature.GetRootSignature());				// シグネチャセット
	list->SetPipelineState(brightnessData_.pso.GetState());										// PSOをセット
	
	// データのバインド
	list->SetGraphicsRootConstantBufferView(0, buffer_.GetGPUView());	// 定数データ
	list->SetGraphicsRootDescriptorTable(1, target->srvInfo_.gpuView_); // 書き込み元テクスチャ
	
	// リソースバリアをセット
	target->ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list);			// 加工元画像
	brightnessData_.rResource.ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);	// 加工対象画像
	
	// 全画面クリア
	brightnessData_.rResource.Clear(list);
	
	// ビューポート設定
	list->RSSetViewports(1, &viewport);
	// シザー矩形設定
	list->RSSetScissorRects(1, &scissorRect);
	
	// 描画を行う
	list->DrawInstanced(3, 1, 0, 0);
	// バリアを元に戻しておく
	brightnessData_.rResource.ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list);

	/// X軸方向にガウシアンブラー
	// コマンドリストに情報をセット
	list->OMSetRenderTargets(1, &gaussX.rResource.rtvInfo_.cpuView_, false, nullptr);	// レンダー対象セット
	list->SetGraphicsRootSignature(gaussX.signature.GetRootSignature());				// シグネチャセット
	list->SetPipelineState(gaussX.pso.GetState());										// PSOをセット

	// リソースバリアをセット
	gaussX.rResource.ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);
	// 全画面クリア
	gaussX.rResource.Clear(list);

	// テクスチャをバインドする
	list->SetGraphicsRootDescriptorTable(0, brightnessData_.rResource.srvInfo_.gpuView_);

	// ビューポート設定
	list->RSSetViewports(1, &viewport);
	// シザー矩形設定
	list->RSSetScissorRects(1, &scissorRect);

	// 描画を行う
	list->DrawInstanced(3, 1, 0, 0);
	// バリアを元に戻しておく
	gaussX.rResource.ChangeResourceBarrier(D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, list);
	
	/// Y軸方向にガウシアンブラー
	// コマンドリストに情報をセット
	list->OMSetRenderTargets(1, &target->rtvInfo_.cpuView_, false, nullptr);	// レンダー対象セット
	list->SetGraphicsRootSignature(gaussX.signature.GetRootSignature());		// シグネチャセット
	list->SetPipelineState(gaussY.GetState());									// PSOをセット

	// リソースバリアをセット
	target->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);

	// テクスチャをバインドする
	list->SetGraphicsRootDescriptorTable(0, gaussX.rResource.srvInfo_.gpuView_);

	// ビューポート設定
	list->RSSetViewports(1, &viewport);
	// シザー矩形設定
	list->RSSetScissorRects(1, &scissorRect);

	// 描画を行う
	list->DrawInstanced(3, 1, 0, 0);
}

void Bloom::DisplayImGui()
{
	// ImGuiを表示
	if (ImGui::TreeNode("Bloom")) {
		// 有効化フラグ
		ImGui::Checkbox("Enable", &use_);

		// 閾値をImGuiでいじる
		ImGui::DragFloat("Threshold", &threshold_, 0.01f, 0.0f, 1.0f);

		// ツリーノード終了
		ImGui::TreePop();
	}
}
