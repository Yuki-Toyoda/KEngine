#include "ImGuiManager.h"

#include "../../Engine/Base/DirectXCommon.h"
#include "../../Engine/Base/WinApp.h"

ImGuiManager* ImGuiManager::GetImstance()
{
	static ImGuiManager instance;
	return &instance;
}

void ImGuiManager::Intialize(WinApp* win, DirectXCommon* dxCommon)
{
	// 引数の値をメンバ変数に代入する
	dxCommon_ = dxCommon;

	// ImGuiのコンテキストを生成
	ImGui::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(win->GetHwnd());
	ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(),
        static_cast<int>(dxCommon_->GetBufferCount()),
		dxCommon_->GetFormat(),
        dxCommon_->GetSRVHeap(),
        dxCommon_->GetSRVHeap()->GetCPUDescriptorHandleForHeapStart(),
        dxCommon_->GetSRVHeap()->GetGPUDescriptorHandleForHeapStart());
}

void ImGuiManager::Finalize()
{
	// ImGuiを落とす
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void ImGuiManager::Begin()
{
	// ImGuiフレーム開始
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void ImGuiManager::End()
{
	// 描画前準備
	ImGui::Render();
}

void ImGuiManager::Draw()
{
    // コマンドリストを取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandManager()->GetRenderCommandList();
	// 描画コマンドを発行
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
}
