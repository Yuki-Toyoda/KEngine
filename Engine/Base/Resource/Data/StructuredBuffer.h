#pragma once
#include "IDataResource.h"
#include "../../DescriptorHeaps/SRV.h"
#include "../../../Utility/Counter/Counter.h"

/// <summary>
/// 構造体バッファリソースクラス
/// </summary>
/// <typeparam name="T">生成する構造体バッファのクラス</typeparam>
template<class T>
class StructuredBuffer : public IDataResource {
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	StructuredBuffer() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxSize">データ最大数</param>
	StructuredBuffer(uint32_t maxSize) : kElementSize_(sizeof(T)), kMaxSize_(maxSize) {};

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~StructuredBuffer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="srv">バッファを生成するSRVヒープ</param>
	void Init(DirectXDevice* device, SRV* srv) {
		// リソース生成
		CreateResource(device, kElementSize_ * kMaxSize_);
		// 生成したリソースのマッピングを行う
		resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

		// SRVの登録設定
		D3D12_SHADER_RESOURCE_VIEW_DESC desc;
		desc.Format						= DXGI_FORMAT_UNKNOWN;						 // フォーマット形式は不明
		desc.Shader4ComponentMapping	= D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // シェーダーからテクスチャにアクセスする際の値指定
		desc.ViewDimension				= D3D12_SRV_DIMENSION_BUFFER;				 // SRVのバッファの設定
		desc.Buffer.FirstElement		= 0;										 // 最初の番号を指定
		desc.Buffer.Flags				= D3D12_BUFFER_SRV_FLAG_NONE;				 // フラッグ設定
		desc.Buffer.NumElements			= kMaxSize_;								 // バッファの最大数を設定
		desc.Buffer.StructureByteStride = kElementSize_;							 // バッファ１つ分のサイズを指定

		// 設定を元にSRVに登録
		info = srv->RegisterStructuredBuffer(resource_.Get(), desc);
	}

	/// <summary>
	/// バッファにデータを登録する関数
	/// </summary>
	/// <param name="t">登録するデータ</param>
	/// <returns>登録したインデックス</returns>
	int AddData(const T& t) {
		// データの最大数を超過していないか確認
		assert(static_cast<uint32_t>(usedCount_.GetCount()) < kMaxSize_);
		// データ追加、およびカウンターを進める
		data_[usedCount_.GetCountAndIncrement()] = t;

		// 追加したデータまでのインデックスを返す
		return usedCount_.GetCount() - 1;
	}

	/// <summary>
	/// カウンターリセット関数
	/// </summary>
	void Reset() { usedCount_.Reset(); }

public: // アクセッサ

	/// <summary>
	/// 使用済みのカウント数をゲッター
	/// </summary>
	/// <returns></returns>
	int GetCount() { usedCount_.GetCount(); }

	/// <summary>
	/// 構造体バッファのサイズゲッター
	/// </summary>
	/// <returns>構造体バッファのサイズ</returns>
	uint32_t GetMaxSize() const { return kMaxSize_; }

	/// <summary>
	/// GPU上のアドレスゲッター
	/// </summary>
	/// <returns>GPU上のアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUView() const { return info.gpuView_; }

	/// <summary>
	/// SRV情報構造体ゲッター
	/// </summary>
	/// <returns>当バッファの情報構造体</returns>
	SRVInfo GetInfo() const { return info; }

public: // パブリックなメンバ変数

	// 実際のデータ
	T* data_ = nullptr;

private: // メンバ変数

	// データ１つ分のサイズ定数
	const uint32_t kElementSize_;
	// データ最大数
	const uint32_t kMaxSize_;

	// SRV上の登録データ
	SRVInfo info;

	// 使用済みカウント用カウンター
	Counter usedCount_;

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};