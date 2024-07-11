#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

#include "../Device/DirectXDevice.h"
#include "../DescriptorHeaps/HeapManager.h"

/// <summary>
/// 全リソース基底クラス
/// </summary>
class IResource
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IResource() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IResource() = default;

protected: // 継承先メンバ変数

	// リソース
	Microsoft::WRL::ComPtr<ID3D12Resource> resource_ = nullptr;

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};

