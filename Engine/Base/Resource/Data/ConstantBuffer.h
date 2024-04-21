#pragma once
#include "IDataResource.h"

/// <summary>
/// 定数バッファリソースクラス
/// </summary>
/// <typeparam name="T">生成する定数バッファのクラス</typeparam>
template<class T>
class ConstantBuffer : public IDataResource
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ConstantBuffer() : kElementSize_(sizeof(T)) {};

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~ConstantBuffer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	void Init(DirectXDevice* device) {
		// リソースの生成を行う
		CreateResource(device, kElementSize_);
		// 生成したリソースのマッピングを行う
		resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));
		// GPU上のアドレスを求める
		view_ = resource_->GetGPUVirtualAddress();
	}

public: // アクセッサ等

	/// <summary>
	/// GPU上のアドレスゲッター
	/// </summary>
	/// <returns>当リソースのGPU上のアドレス</returns>
	D3D12_GPU_VIRTUAL_ADDRESS GetGPUView() { return view_; }

public: // パブリックなメンバ変数

	// データ本体
	T* data_ = nullptr;

private: // メンバ変数

	// データ１つ分のサイズ定数
	const uint32_t kElementSize_{};

	// リソースまでのGPU上のアドレス
	D3D12_GPU_VIRTUAL_ADDRESS view_{};

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};
