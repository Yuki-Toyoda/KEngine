#include "DirectXDevice.h"
#include "../../Debug/Debug.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

using namespace Microsoft::WRL;

void DirectXDevice::Init()
{
	// 結果確認用
	HRESULT result = S_FALSE;

#ifdef _DEBUG // デバッグ時のみ有効
	// デバックレイヤーの有効処理
	ID3D12Debug1* debugController = nullptr;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
		// デバックレイヤーを有効にする
		debugController->EnableDebugLayer();
		// さらにGPU側でもチェックを行うようにする
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
#endif // _DEBUG

	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory_)); // 生成
	assert(SUCCEEDED(result)); // 生成出来たか確認
	
	// 使用するアダプタ格納変数
	ComPtr<IDXGIAdapter4> useAdapter;
	// アダプターを性能がいい順から列挙する
	for (UINT i = 0; dxgiFactory_->EnumAdapterByGpuPreference(
		i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; i++
		) {

		// アダプタ情報取得
		DXGI_ADAPTER_DESC3 adapterDesc{}; // アダプタ情報格納
		result = useAdapter->GetDesc3(&adapterDesc); // 取得
		assert(SUCCEEDED(result)); // 確認
		// ソフトウェアアダプタでなければ採用する
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			// 採用したアダプターの情報をログに入れて出力する
			Debug::Log(Debug::ConvertString(std::format(L"use Adapter:{}\n", adapterDesc.Description)));
			// 処理を抜ける
			break;
		}
		// ソフトウェアアダプタの場合は無視する
		useAdapter = nullptr;
	}
	// 適切なアダプタが取得出来なかった場合は起動しない
	assert(useAdapter != nullptr);

	// 機能レベル
	D3D_FEATURE_LEVEL featureLevels[] = {
		D3D_FEATURE_LEVEL_12_2,
		D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0
	};
	// ログ出力用文字列
	const char* featureLevelStriings[] = { "12.2", "12.1",  "12.0" };

	// 取得したアダプターを用いて、高い順にデバイスを生成できるか試す
	for (size_t i = 0; i < _countof(featureLevels); ++i) {
		// 採用したアダプタを使用してデバイスを生成
		result = D3D12CreateDevice(useAdapter.Get(), featureLevels[i], IID_PPV_ARGS(&device_));
		// 指定した機能レベルでデバイスが生成出来ているかを確認する
		if (SUCCEEDED(result)) {
			// ログを出力
			Debug::Log(std::format("FeatureLevel : {}\n", featureLevelStriings[i]));
			// 生成できていればループ処理を抜ける
			break;
		}
	}

	// デバイスを生成出来なかった場合は起動しない
	assert(device_ != nullptr);
	// 初期化完了のログを出す
	Debug::Log("Complete Create D3D12Device!!!\n");

#ifdef _DEBUG // デバッグ時のみ有効
	// デバックでの実行時、エラーが出た場合警告を出す
	ID3D12InfoQueue* infoQueue;

	if (SUCCEEDED(device_->QueryInterface(IID_PPV_ARGS(&infoQueue)))) {
		// 重大なエラー時に停止する
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		// エラー時に停止する
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		// 警告時に停止する
		infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_WARNING, true);

		// 抑制するエラーID
		D3D12_MESSAGE_ID denyIds[] = {
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};

		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;

		// 指定したエラーの表示を抑制する
		infoQueue->PushStorageFilter(&filter);

		// 解放する
		infoQueue->Release();
	}
#endif // _DEBUG
}
