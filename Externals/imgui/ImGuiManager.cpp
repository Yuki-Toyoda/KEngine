#include "ImGuiManager.h"

#include "implot.h"
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
    // ImPlotのコンテキストを生成
    ImPlot::CreateContext();
	// ImGuiのスタイルを設定
	ImGui::StyleColorsDark();
	// プラットフォームとレンダラーのバックエンドを設定する
	ImGui_ImplWin32_Init(win->GetHwnd());
    // SRV上に情報登録、インデックスの取得
    srvInfo_ = dxCommon->GetSRV()->CreateImGuiSpace();
    // ImGuiの初期化
    ImGui_ImplDX12_Init(
		dxCommon_->GetDevice(),
        static_cast<int>(dxCommon_->GetBufferCount()),
		dxCommon_->GetFormat(),
        dxCommon_->GetSRV()->GetDescriptorHeap(),
        srvInfo_.cpuView_,
        srvInfo_.gpuView_);
}

void ImGuiManager::Finalize()
{
	// ImGuiを落とす
	ImGui_ImplDX12_Shutdown();
	ImGui_ImplWin32_Shutdown();
    ImPlot::DestroyContext();
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
