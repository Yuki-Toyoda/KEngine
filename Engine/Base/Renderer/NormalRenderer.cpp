#include "NormalRenderer.h"
#include "../../../Externals/imgui/ImGuiManager.h"
#include "../../Primitive/PrimitiveManager.h"
#include "../../Lighting/Light/DirectionalLight.h"
#include "../../Base/DirectXCommon.h"

void NormalRenderer::Init(DirectXDevice* device, DXC* dxc, PrimitiveManager* pm, DirectionalLight* lt)
{
	// ルートシグネチャマネージャの取得
	RootSignatureManager* rtsManager = RootSignatureManager::GetInstance();

	// ルートシグネチャ取得
	standardRootSignature_ = rtsManager->GetRootSignature(0); // 通常
	skinRootSignature_	   = rtsManager->GetRootSignature(1); // スキンアニメーション

	// 通常描画用PSO初期化
	standardPSO_.Init(standardRootSignature_, dxc)
		.SetMeshShader("Engine/Resource/Shader/MeshletMS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/MeshletPS.hlsl")
		.Build(device->GetDevice());

	// スキンアニメーション描画用PSO初期化
	skinModelPSO_.Init(skinRootSignature_, dxc)
		.SetMeshShader("Engine/Resource/Shader/SkinMeshlet/MeshletSkinMS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/SkinMeshlet/MeshletSkinPS.hlsl")
		.Build(device->GetDevice());

	// 形状マネージャのインスタンス取得
	primitiveManager_ = pm;

	// 平行光源の取得
	light_ = lt;
}

void NormalRenderer::DrawCall(ID3D12GraphicsCommandList6* list)
{
	// 形状マネージャの更新
	primitiveManager_->Update();

	// ImGuiの受付終了
	ImGui::EndFrame();

	// ターゲット分ループする
	for (std::vector<Target>::iterator it = targets_.begin(); it != targets_.end(); it++) {
		// 描画先のRTVとDSVをセットする
		list->OMSetRenderTargets(1, &it->backBuffer_->rtvInfo_.cpuView_, false, &it->depthBuffer_->dsvInfo_.cpuView_);

		// リソースバリアをセットする
		D3D12_RESOURCE_STATES beforeBarrier = it->backBuffer_->GetBarrier();
		// リソースバリア変更
		it->backBuffer_->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);
		// 全画面クリア
		it->backBuffer_->Clear(list);
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

		// コマンドリストに通常描画ルートシグネチャを設定
		list->SetGraphicsRootSignature(standardRootSignature_);

		// コマンドリストに通常描画PSOを設定
		list->SetPipelineState(standardPSO_.GetState());

		// SRVヒープの取得
		SRV* s = DirectXCommon::GetInstance()->GetSRV();

		// リスト取得
		const auto& p = primitiveManager_->GetPrimitives();
		// 全形状を描画
		for (const auto& primitive : p) {
			// 形状を描画するなら
			if (primitive->isActive_) {
				if (primitive->isAnimated_) {
					// コマンドリストにスキンアニメーション描画ルートシグネチャを設定
					list->SetGraphicsRootSignature(skinRootSignature_);
					// コマンドリストにスキンアニメーション描画PSOを設定
					list->SetPipelineState(skinModelPSO_.GetState());
					// 共通データのアドレスを渡す
					list->SetGraphicsRootConstantBufferView(0, it->view_);		   // カメラデータ
					list->SetGraphicsRootConstantBufferView(1, light_->view());    // 平行光源データ
					list->SetGraphicsRootDescriptorTable(9, s->GetFirstTexView()); // テクスチャデータ
				}
				else {
					// コマンドリストに通常描画ルートシグネチャを設定
					list->SetGraphicsRootSignature(standardRootSignature_);
					// コマンドリストに通常描画PSOを設定
					list->SetPipelineState(standardPSO_.GetState());
					// 共通データのアドレスを渡す
					list->SetGraphicsRootConstantBufferView(0, it->view_);		   // カメラデータ
					list->SetGraphicsRootConstantBufferView(1, light_->view());	   // 平行光源データ
					list->SetGraphicsRootDescriptorTable(8, s->GetFirstTexView()); // テクスチャデータ
				}

				// 描画処理を実行
				primitive->Draw(list);
			}
		}

		// 最後のイテレータでImGuiを描画する
		if (it == std::prev(targets_.end())) {
			// ImGuiを描画
			ImGui::Render();
			ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list);
		}

		// バリアを元に戻す
		it->backBuffer_->ChangeResourceBarrier(beforeBarrier, list);
	}
}

void NormalRenderer::Reset()
{
	// ターゲット配列クリア
	targets_.clear();
}
