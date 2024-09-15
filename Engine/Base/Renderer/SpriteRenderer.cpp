#include "SpriteRenderer.h"
#include "../../Model/ModelManager.h"
#include "../../Scene/SceneManager.h"
#include "../../Base/DirectXCommon.h"

void SpriteRenderer::Init(DirectXDevice* device, DXC* dxc, ModelManager* mm)
{
	// ルートシグネチャマネージャの取得
	RootSignatureManager* rtsManager = RootSignatureManager::GetInstance();

	// ルートシグネチャ取得
	spriteRootSignature_ = rtsManager->GetRootSignature(2); // スプライト

	// スプライト描画用PSO初期化
	spritePSO_.Init(spriteRootSignature_, dxc)
		.SetMeshShader("Engine/Resource/Shader/SpriteMeshlet/MeshletSpriteMS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/SpriteMeshlet/MeshletSpritePS.hlsl")
		.Build(device->GetDevice());

	// 形状マネージャのインスタンス取得
	modelManager_ = mm;
}

void SpriteRenderer::DrawCall(ID3D12GraphicsCommandList6* list)
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

		// コマンドリストにスプライト描画ルートシグネチャを設定
		list->SetGraphicsRootSignature(spriteRootSignature_);
		// コマンドリストにスプライト描画PSOを設定
		list->SetPipelineState(spritePSO_.GetState());
		// 共通データのアドレスを渡す
		list->SetGraphicsRootConstantBufferView(0, it->view_);		   // カメラデータ
		list->SetGraphicsRootDescriptorTable(7, s->GetFirstTexView()); // テクスチャデータ

		// スプライトモデルの描画を行う
		modelManager_->SpriteModelDraw(list);

		// バリアを元に戻す
		it->backBuffer_->ChangeResourceBarrier(beforeBarrier, list);
	}
}

void SpriteRenderer::Reset()
{
	// ターゲット配列クリア
	targets_.clear();
}
