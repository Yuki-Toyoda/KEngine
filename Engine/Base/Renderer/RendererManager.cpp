#include "RendererManager.h"
#include "../../Resource/Texture/TextureManager.h"

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
	normalRenderer_.Init(device, &dxc_, modelManager_, light_.get()); // 通常描画レンダラー
	ppRenderer_.Init();												  // ポストプロセスレンダラー

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

	// 通常描画を行う
	normalRenderer_.DrawCall(list);
	// ポストプロセス描画を行う
	ppRenderer_.DrawCall(list);

	// コマンドリストの実行
	command_.Execute();

	// 各レンダラーをリセット
	normalRenderer_.Reset(); // 通常描画
	ppRenderer_.Reset();	 // ポストプロセス描画
}
