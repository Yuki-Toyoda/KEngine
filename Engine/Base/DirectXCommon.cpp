#include "DirectXCommon.h"
#include <cassert>

#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "d3dCompiler.lib")

#include "../GameObject/Core/Camera.h"

using namespace Microsoft::WRL;
using namespace KEngine;

DirectXCommon* DirectXCommon::GetInstance() {
	// クラスのインスタンスを取得
	static DirectXCommon instance;
	// インスタンスを返す
	return &instance;
}

void DirectXCommon::Init(WinApp* win)
{
	// 結果確認用
	HRESULT result = S_FALSE;

	// 引数のNULLチェックを行う
	assert(win);

	// 引数の値をメンバ変数に代入
	winApp_ = win;

	// デバイスの生成
	dxDevice_ = std::make_unique<DirectXDevice>(); // 生成
	dxDevice_->Init();							   // 初期化
	device_ = dxDevice_->GetDevice();			   // デバイスをメンバ変数に代入

	// ルートシグネチャマネージャの生成、および初期化
	rtsManger_ = RootSignatureManager::GetInstance();
	rtsManger_->SetDevice(device_);
	rtsManger_->Init();

	// HeapManagerの生成
	heaps_ = std::make_unique<HeapManager>(dxDevice_.get());

	// 描画レンダラーマネージャー初期化
	rendererManager_ = std::make_unique<RendererManager>();
	rendererManager_->Init(dxDevice_.get(), heaps_->srv());

	// スワップチューンの生成
	swapChainDesc_.Width			= Config::Window::KWindowWidth;    // 画面の横幅
	swapChainDesc_.Height			= Config::Window::KWindowHeight;   // 画面の縦幅
	swapChainDesc_.Format			= DXGI_FORMAT_R8G8B8A8_UNORM;	   // 色の形式
	swapChainDesc_.SampleDesc.Count = 1;							   // マルチサンプルしない
	swapChainDesc_.BufferUsage		= DXGI_USAGE_RENDER_TARGET_OUTPUT; // 描画ターゲットとする
	swapChainDesc_.BufferCount		= 2;							   // ダブルバッファ
	swapChainDesc_.SwapEffect		= DXGI_SWAP_EFFECT_FLIP_DISCARD;   // モニターに描画した時点で中身を廃棄する

	// コマンドキューやウィンドウハンドルや設定を渡して生成する
	ComPtr<IDXGISwapChain1> swapChain1;																				  // スワップチューン用構造体を生成
	result = dxDevice_->GetDXGIFactory()->CreateSwapChainForHwnd(
		rendererManager_->GetCommand()->Queue(), winApp_->GetHwnd(), &swapChainDesc_, nullptr, nullptr, &swapChain1); // スワップチューンを生成
	assert(SUCCEEDED(result));																						  // 生成確認
	// SwapChain4に変換、メンバ変数に代入
	swapChain1->QueryInterface(IID_PPV_ARGS(&swapChain_));

	// バックバッファの生成
	for (int i = 0; i < 2; i++) {
		// バックバッファの生成、配列に追加
		backBuffers_.push_back(BackBuffer(swapChain_.Get(), i));
		// 生成したバッファの初期化
		backBuffers_.back().Init(dxDevice_.get(), heaps_.get());
	}

	// DSVリソースの生成
	depthStencil_.Init(dxDevice_.get(), heaps_.get());

	// FPS固定を初期化
	InitializeFixFPS();
}

void DirectXCommon::Draw()
{
	// 描画処理を行う
	rendererManager_->DrawCall();

	// GPUとOSに画面の交換を行うように指示する
	swapChain_->Present(0, 0);

	// FPSの固定
	UpdateFixFPS();
}

void DirectXCommon::SetMainCamera(Camera* camera)
{
	// 描画マネージャーに描画ターゲットセット
	rendererManager_->AddTarget(camera->GetBufferView(), camera->GetRendererResource(), &depthStencil_);
	// ポストプロセスマネージャに描画ターゲットセット
	rendererManager_->AddTarget(camera->GetRendererResource(), &backBuffers_[swapChain_->GetCurrentBackBufferIndex()], camera->GetDepthStencil());
}

void DirectXCommon::InitializeFixFPS()
{
	// 初期化時点での現在時間を記録する
	reference_ = std::chrono::steady_clock::now();
}

void DirectXCommon::UpdateFixFPS()
{
	/// 定数設定
	// 1/60秒ぴったりの時間
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	// 1/60秒よりわずかに短い時間
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));

	// 現在時間の取得
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	// 前回の記録からの経過時間を取得
	std::chrono::microseconds elapsed =
		std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);

	// 1/60秒経っていない場合
	if (elapsed < kMinCheckTime) {
		// 1/60秒経過するまで微少なスリープを繰り返す
		while (std::chrono::steady_clock::now() - reference_ < kMinTime){}
		{
			// 1マイクロ秒スリープさせる
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}
	}
	// 現在の時間を記録する
	reference_ = std::chrono::steady_clock::now();

}
