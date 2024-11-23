#include "ModelRenderer.h"
#include "Engine/Model/ModelManager.h"
#include "Engine/Particle/ParticleManager.h"
#include "Engine/Lighting/Light/DirectionalLight.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Base/DirectXCommon.h"

void ModelRenderer::Init(DirectXDevice* device, DXC* dxc, ModelManager* mm, DirectionalLight* lt)
{
	// ルートシグネチャマネージャの取得
	RootSignatureManager* rtsManager = RootSignatureManager::GetInstance();

	// ルートシグネチャ取得
	standardRootSignature_ = rtsManager->GetRootSignature(0); // 通常
	skinRootSignature_	   = rtsManager->GetRootSignature(1); // スキンアニメーション
	particleRootSignature_ = rtsManager->GetRootSignature(3); // パーティクル
	skyBoxRootSignature_   = rtsManager->GetRootSignature(4); // スカイボックス
	
	// 通常描画用PSO初期化
	standardPSO_.Init(standardRootSignature_, dxc)
		.SetMeshShader("Engine/Resource/Shader/MeshletMS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/MeshletToonPS.hlsl")
		.Build(device->GetDevice());

	// スキニング計算用PSO初期化
	skinModelPSO_.Init(skinRootSignature_, dxc, PSO::PSOType::Compute)
		.SetComputeShader("Engine/Resource/Shader/Compute/Skining.CS.hlsl")
		.Build(device->GetDevice());

	// パーティクル描画用PSO初期化
	particlePSO_.Init(particleRootSignature_, dxc)
		.SetBlendState(1)
		.SetDepthStencilState(true, false)
		.SetTaskShader("Engine/Resource/Shader/Particle/ParticleAS.hlsl")
		.SetMeshShader("Engine/Resource/Shader/Particle/ParticleMS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/Particle/ParticlePS.hlsl")
		.Build(device->GetDevice());

	// スカイボックス描画用PSO初期化
	skyBoxPSO_.Init(skyBoxRootSignature_, dxc, PSO::PSOType::Vertex)
		.SetVertexShader("Engine/Resource/Shader/SkyBox/SkyBox.VS.hlsl")
		.SetPixelShader("Engine/Resource/Shader/SkyBox/SkyBox.PS.hlsl")
		.SetDepthStencilState(1, 0)
		.Build(device->GetDevice());

	// 形状マネージャのインスタンス取得
	modelManager_ = mm;

	// パーティクルマネージャーのインスタンス取得
	particleManager_ = ParticleManager::GetInstance();
	particleManager_->Init()
		.CreateParticlePSO("Test", "Test/InitTestParticle.CS.hlsl", "Test/EmitTestParticle.CS.hlsl", "Test/UpdateTestParticle.CS.hlsl")
		.CreateParticlePSO("Charge", "Test/InitTestParticle.CS.hlsl", "Charge/EmitChargeParticle.CS.hlsl", "Charge/UpdateChargeParticle.CS.hlsl")
		.CreateParticlePSO("EnemyHit", "Test/InitTestParticle.CS.hlsl", "EnemyHit/EmitEnemyHitParticle.CS.hlsl", "Hit/UpdateHitParticle.CS.hlsl")
		.CreateParticlePSO("Hit", "Test/InitTestParticle.CS.hlsl", "Hit/EmitHitParticle.CS.hlsl", "Hit/UpdateHitParticle.CS.hlsl")
		.CreateParticlePSO("Enemy", "Test/InitTestParticle.CS.hlsl", "Enemy/EmitEnemyParticle.CS.hlsl", "Enemy/UpdateEnemyParticle.CS.hlsl")
		.CreateParticlePSO("BulletTrail", "Test/InitTestParticle.CS.hlsl", "BulletTrail/EmitBulletTrailParticle.CS.hlsl", "BulletTrail/UpdateBulletTrailParticle.CS.hlsl");

	// 平行光源の取得
	light_ = lt;
}

void ModelRenderer::DrawCall(ID3D12GraphicsCommandList6* list)
{
	// 形状マネージャの更新
	modelManager_->Update();

	// コマンドリストにスキニング描画ルートシグネチャを設定
	list->SetComputeRootSignature(skinRootSignature_);
	// コマンドリストにスキニング描画PSOを設定
	list->SetPipelineState(skinModelPSO_.GetState());

	// スキニング計算を開始する
	modelManager_->ExecuteComputeSkiningModel(list);

	// パーティクルマネージャーの更新
	particleManager_->Update();

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

		// テクスチャ用にSRVヒープの取得
		SRV* s = DirectXCommon::GetInstance()->GetSRV();

		// 環境マップ用にスカイボックスの取得
		SkyBox* skyBox = SceneManager::GetInstance()->GetCurrentScene()->skyBox_.get();

		// コマンドリストにスカイボックス描画ルートシグネチャを設定
		list->SetGraphicsRootSignature(skyBoxRootSignature_);
		// コマンドリストにスプライト描画PSOを設定
		list->SetPipelineState(skyBoxPSO_.GetState());
		// 共通データのアドレスを渡す
		list->SetGraphicsRootConstantBufferView(0, it->view_);

		// スカイボックスの描画を行う
		skyBox->Draw(list);

		// コマンドリストに通常描画ルートシグネチャを設定
		list->SetGraphicsRootSignature(standardRootSignature_);
		// コマンドリストに通常描画PSOを設定
		list->SetPipelineState(standardPSO_.GetState());
		// 共通データのアドレスを渡す
		list->SetGraphicsRootConstantBufferView(0, it->view_);					// カメラデータ
		list->SetGraphicsRootConstantBufferView(1, light_->view());				// 平行光源データ
		list->SetGraphicsRootDescriptorTable(9, skyBox->GetTextureAddress());	// 環境マップ用テクスチャ
		list->SetGraphicsRootDescriptorTable(10, s->GetFirstTexView());			// テクスチャデータ

		// 通常モデルの描画を行う
		modelManager_->NormalModelDraw(list);

		// 通常描画でスキニングモデルの描画を行う
		modelManager_->SkiningModelDraw(list);

		// コマンドリストにパーティクル描画ルートシグネチャを設定
		list->SetGraphicsRootSignature(particleRootSignature_);
		// コマンドリストにパーティクル描画PSOを設定
		list->SetPipelineState(particlePSO_.GetState());
		// 共通データのアドレスを渡す
		list->SetGraphicsRootConstantBufferView(0, it->view_);		   // カメラデータ
		list->SetGraphicsRootConstantBufferView(1, light_->view());	   // 平行光源データ
		list->SetGraphicsRootDescriptorTable(9, s->GetFirstTexView()); // テクスチャデータ

		// パーティクルモデルの描画を行う
		particleManager_->Draw();

		// バリアを元に戻す
		it->backBuffer_->ChangeResourceBarrier(beforeBarrier, list);
	}
}

void ModelRenderer::Reset()
{
	// ターゲット配列クリア
	targets_.clear();
}
