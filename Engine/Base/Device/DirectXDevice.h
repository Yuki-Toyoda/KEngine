#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>

#include <wrl.h>

/// <summary>
/// DirextX12のデバイス
/// </summary>
class DirectXDevice
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

public: // アクセッサ等
	/// <summary>
	/// デバイスゲッター
	/// </summary>
	/// <returns>デバイス</returns>
	ID3D12Device* GetDevice() { return device_.Get(); }

	/// <summary>
	/// DXGIファクトリーゲッター
	/// </summary>
	/// <returns>DXGIファクトリー</returns>
	IDXGIFactory7* GetDXGIFactory() { return dxgiFactory_.Get(); }
private: // メンバ変数
	// デバイス本体
	Microsoft::WRL::ComPtr<ID3D12Device> device_;
	// DXGIファクトリー
	Microsoft::WRL::ComPtr<IDXGIFactory7> dxgiFactory_;
};


