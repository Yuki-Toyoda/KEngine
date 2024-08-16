#pragma once
#include "IDataResource.h"
#include "../../DescriptorHeaps/SRV.h"
#include "../../../Utility/Counter/Counter.h"

/// <summary>
/// GPUから書き込みが可能な構造体バッファ
/// </summary>
/// <typeparam name="T">生成するバッファのクラス</typeparam>
template<class T>
class RWStructuredBuffer : public IDataResource
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	RWStructuredBuffer() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxSize">データ最大数</param>
	RWStructuredBuffer(uint32_t maxSize) : kElementSize_(sizeof(T)), kMaxSize_(maxSize) {};

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~RWStructuredBuffer() = default;

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
		//resource_->Map(0, nullptr, reinterpret_cast<void**>(&data_));

		// UAVの登録設定
		D3D12_UNORDERED_ACCESS_VIEW_DESC uDesc{};
		uDesc.Format						= DXGI_FORMAT_UNKNOWN;		  // フォーマット形式は不明
		uDesc.ViewDimension					= D3D12_UAV_DIMENSION_BUFFER; // UAVのバッファの設定
		uDesc.Buffer.Flags					= D3D12_BUFFER_UAV_FLAG_NONE; // GPUからアクセス可能な設定
		uDesc.Buffer.FirstElement			= 0;						  // 最初の番号を指定
		uDesc.Buffer.NumElements			= kMaxSize_;				  // バッファの最大数の設定
		uDesc.Buffer.StructureByteStride	= kElementSize_;			  // バッファ１つ分のサイズを指定

		// 設定を元にUAV情報をSRVに登録
		uavInfo_ = srv->RegisterRWStructuredBuffer(resource_.Get(), uDesc);

		// SRVの登録設定
		D3D12_SHADER_RESOURCE_VIEW_DESC sDesc{};
		sDesc.Format = DXGI_FORMAT_UNKNOWN;											 // フォーマット形式は不明
		sDesc.Shader4ComponentMapping	 = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING; // シェーダーからテクスチャにアクセスする際の値指定
		sDesc.ViewDimension				 = D3D12_SRV_DIMENSION_BUFFER;				 // SRVのバッファの設定
		sDesc.Buffer.FirstElement		 = 0;										 // 最初の番号を指定
		sDesc.Buffer.Flags				 = D3D12_BUFFER_SRV_FLAG_NONE;				 // フラッグ設定
		sDesc.Buffer.NumElements		 = kMaxSize_;								 // バッファの最大数を設定
		sDesc.Buffer.StructureByteStride = kElementSize_;							 // バッファ１つ分のサイズを指定

		// 設定を元にSRV情報SRVに登録
		srvInfo_ = srv->RegisterStructuredBuffer(resource_.Get(), sDesc);
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
	/// GPU上のUAV情報へのアドレスゲッター
	/// </summary>
	/// <returns>GPU上のUAV情報へのアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetUAVView() const { return uavInfo_.gpuView_; }

	/// <summary>
	/// GPU上のSRV情報へのアドレスゲッター
	/// </summary>
	/// <returns>GPU上のSRV情報へのアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVView() const { return srvInfo_.gpuView_; }

	/// <summary>
	/// UAV情報構造体ゲッター
	/// </summary>
	/// <returns>当バッファの情報構造体</returns>
	UAVInfo GetUAVInfo() const { return uavInfo_; }

	/// <summary>
	/// UAV情報構造体ゲッター
	/// </summary>
	/// <returns>当バッファの情報構造体</returns>
	SRVInfo GetSRVInfo() const { return srvInfo_; }

	/// <summary>
	/// リソース本体のゲッター
	/// </summary>
	/// <returns>リソース本体</returns>
	ID3D12Resource* GetResource() { return resource_.Get(); }

private: // プライベートなメンバ関数

	/// <summary>
	/// リソース生成関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="size">リソースのサイズ</param>
	void CreateResource(DirectXDevice* device, size_t size) override {
		// 結果確認用
		HRESULT result = S_FALSE;

		/// ヒープの設定
		// ヒープ設定用構造体を定義
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};
		// GPUにアップロードする設定
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

		/// リソースの設定を行う
		// リソース設定用構造体を定義
		D3D12_RESOURCE_DESC resourceDesc{};
		resourceDesc.Dimension		  = D3D12_RESOURCE_DIMENSION_BUFFER; // バッファリソース用の設定
		resourceDesc.Width			  = size;							 // リソースのサイズ設定
		resourceDesc.Height			  = 1;								 // バッファの場合は1
		resourceDesc.DepthOrArraySize = 1;								 // バッファの場合は1
		resourceDesc.MipLevels		  = 1;								 // バッファの場合は1
		resourceDesc.SampleDesc.Count = 1;								 // バッファの場合は1
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;			 // バッファの場合はこうする決まり
		resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS; // GPUからアクセスできる設定

		// バッファリソースを生成する
		result = device->GetDevice()->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_COMMON,
			nullptr,
			IID_PPV_ARGS(&resource_));

		// 生成に失敗していた場合停止させる
		assert(SUCCEEDED(result));
	}

public: // パブリックなメンバ変数

	// 実際のデータ
	T* data_ = nullptr;

private: // メンバ変数

	// データ１つ分のサイズ定数
	const uint32_t kElementSize_;
	// データ最大数
	const uint32_t kMaxSize_;

	// SRV上の登録データ
	UAVInfo uavInfo_; // UAVの情報
	SRVInfo srvInfo_; // SRVの情報

	// 使用済みカウント用カウンター
	Counter usedCount_;

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}
};