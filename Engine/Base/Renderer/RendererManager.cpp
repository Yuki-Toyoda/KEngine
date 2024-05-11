#include "RendererManager.h"

void RendererManager::Init(DirectXDevice* device, SRV* srv)
{
	// 引数からSRVを取得
	srv_ = srv;

	// コマンド管理クラスの初期化
	command_.Init(device);

	// 形状マネージャの生成と初期化
	primitiveManager_ = PrimitiveManager::GetInstance(); // 生成
	primitiveManager_->Init();							 // 初期化

	// ライトの生成を行う
	light_ = std::make_unique<DirectionalLight>("MainLight"); // 生成
	light_->Init();											  // 初期化

	// 通常描画レンダラーを初期化
	normalRenderer_.Init(device, &dxc_, primitiveManager_, light_.get());
}

void RendererManager::DrawCall()
{
	// ライトを更新しておく
	light_->Update();

	// 平行光源用のImGuiを表示
	ImGui::Begin("Light");
	light_->DisplayImGui();
	ImGui::End();

	// リストをポインタ化して取得
	ID3D12GraphicsCommandList6* list = command_.List();

	// コマンドリストにSRVのDescriptorHeapを設定する
	ID3D12DescriptorHeap* descriptorHeaps[] = { srv_->GetDescriptorHeap() };
	list->SetDescriptorHeaps(1, descriptorHeaps);

	// 通常描画を行う
	normalRenderer_.DrawCall(list);

	// コマンドリストの実行
	command_.Execute();
}
