#include "DirectXCommon.h"
#include <cassert>

#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, "d3dCompiler.lib")

DirectXCommon* DirectXCommon::GetInstance() {
	// クラスのインスタンスを取得
	static DirectXCommon instance;
	// インスタンスを返す
	return &instance;
}

void DirectXCommon::Init(WinApp* win,
	int32_t backBufferWidth, int32_t backBufferHeight) {

	// 引数のNULLチェックを行う
	assert(win);

	// 引数の値をメンバ変数に代入
	winApp_ = win;
	backBufferWidth_ = backBufferWidth;
	backBufferHeight_ = backBufferHeight;

	// デバイスの生成
	dxDevice_ = std::make_unique<DirectXDevice>(); // インスタンス生成
	dxDevice_->Init();					   // 初期化
	device_ = dxDevice_->GetDevice();			   // デバイスを渡す

	// コマンドマネージャー生成
	commandManager_ = std::make_unique<CommandManager>(); // インスタンス生成
	commandManager_->Init(device_); // 初期化

	// 各種ヒープの生成
	rtv_ = std::make_unique<RTV>();																							 // インスタンス生成
	rtv_->Init(winApp_->GetHwnd(), dxDevice_.get(), backBufferWidth_, backBufferHeight_, commandManager_->GetQueue()); // 初期化
	srv_ = std::make_unique<SRV>();																						     // インスタンス生成
	srv_->Init(dxDevice_->GetDevice());																				 // 初期化
	dsv_ = std::make_unique<DSV>();																						     // インスタンス生成
	dsv_->Init(dxDevice_->GetDevice(), backBufferWidth_, backBufferHeight_);											 // 初期化

	// コマンドマネージャーにヒープをセット
	commandManager_->SetHeaps(rtv_.get(), srv_.get(), dsv_.get(), L"Engine/Resource/Shader/Object3D.VS.hlsl", L"Engine/Resource/Shader/Object3D.PS.hlsl");

	// FPS初期化
	InitializeFixFPS();

}


void DirectXCommon::Draw()
{
	// ドローコール
	commandManager_->DrawCall();
}

void DirectXCommon::PostDraw() {

	// 描画後処理
	commandManager_->PostDraw();
	// FPSの固定
	UpdateFixFPS();
	// 描画数リセット
	commandManager_->Reset();
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
