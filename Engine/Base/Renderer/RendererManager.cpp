#include "RendererManager.h"
#include "Engine/Resource/Texture/TextureManager.h"
#include "Externals/imgui/ImGuiManager.h"
#include "Engine/Base/DirectXCommon.h"

void RendererManager::Init(DirectXDevice* device, SRV* srv)
{
	// 引数からSRVを取得
	srv_ = srv;

	// コマンド管理クラスの初期化
	command_.Init(device);

	// テクスチャマネージャ初期化
	TextureManager::GetInstance()->Init();

	// 形状マネージャの生成と初期化
	modelManager_ = ModelManager::GetInstance(); // 生成
	modelManager_->Init();						 // 初期化

	// ライトの生成を行う
	light_ = std::make_unique<DirectionalLight>("MainLight"); // 生成
	light_->Init({ 0.45f, 0.6f, 1.0f, 1.0f }, { 0.55f, -1.0f, -0.55f }, 1.0f); // 初期化

	// レンダラー達を初期化
	normalRenderer_.Init(device, &dxc_, modelManager_, light_.get()); // 3D描画レンダラー
	ppRenderer_.Init();												  // ポストプロセスレンダラー
	spriteRenderer_.Init(device, &dxc_, modelManager_);				  // 2D描画レンダラー
}

void RendererManager::DrawCall()
{
	// ライトを更新しておく
	light_->Update();

#ifdef _DEBUG

	// 平行光源用のImGuiを表示
	ImGui::Begin("Light");
	light_->DisplayImGui();
	ImGui::End();

#endif // _DEBUG

	// リストをポインタ化して取得
	ID3D12GraphicsCommandList6* list = command_.List();

	// 形状を設定する
	list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// コマンドリストにSRVのDescriptorHeapを設定する
	ID3D12DescriptorHeap* descriptorHeaps[] = { srv_->GetDescriptorHeap() };
	list->SetDescriptorHeaps(1, descriptorHeaps);

	// 3D描画を行う
	normalRenderer_.DrawCall(list);
	// ポストプロセス描画を行う
	ppRenderer_.DrawCall(list);
	// 2D描画を行う
	spriteRenderer_.DrawCall(list);

	// ImGuiの描画を行う
	ImGuiDraw(list);

	// コマンドリストの実行
	command_.Execute();

	// 各レンダラーをリセット
	normalRenderer_.Reset(); // 3D描画
	ppRenderer_.Reset();	 // ポストプロセス描画
	spriteRenderer_.Reset(); // 2D描画
}

void RendererManager::ImGuiDraw(ID3D12GraphicsCommandList6* list)
{
	// 現在書き込み中のバッファを取得
	BackBuffer* b = DirectXCommon::GetInstance()->GetCurrentBackBuffer();
	// バリアの以前のステートを取得しておく
	D3D12_RESOURCE_STATES beforeBarrier = b->GetBarrier();
	// 書き込み対象にバックバッファを変更
	b->ChangeResourceBarrier(D3D12_RESOURCE_STATE_RENDER_TARGET, list);

	//ビューポートの設定
	D3D12_VIEWPORT viewport = {};
	// ウィンドウのクライアント領域のサイズと一致させて画面全体に表示
	viewport.Width = static_cast<float>(b->width_);   // 画面横幅
	viewport.Height = static_cast<float>(b->height_); // 画面縦幅
	viewport.TopLeftX = 0;							  // 画面左上X座標
	viewport.TopLeftY = 0;							  // 画面左上Y座標
	viewport.MinDepth = 0.0f;						  // 最小深度
	viewport.MaxDepth = 1.0f;						  // 最大深度
	// コマンドリストにビューポートを設定
	list->RSSetViewports(1, &viewport);

	// シザー矩形の設定
	D3D12_RECT scissorRect = {};
	// ビューポートと同一の矩形が表示
	scissorRect.left = 0;			 // 左
	scissorRect.right = b->width_;   // 右
	scissorRect.top = 0;			 // 上
	scissorRect.bottom = b->height_; // 下
	// コマンドリストにシザー矩形を設定
	list->RSSetScissorRects(1, &scissorRect);

	// ImGuiの受付終了
	ImGui::EndFrame();

	// ImGuiを描画
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), list);

	// バリアを元に戻す
	b->ChangeResourceBarrier(beforeBarrier, list);
}
