#pragma once
#include "../IResource.h"

/// <summary>
/// シェーダー用リソース基底クラス
/// </summary>
class IDataResource : public IResource
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IDataResource() = default;

	/// <summary>
	/// 仮想デストラクタ
	/// </summary>
	virtual ~IDataResource() = default;

protected: // 継承先メンバ関数

	/// <summary>
	/// リソース生成関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="size">リソースのサイズ</param>
	void CreateResource(DirectXDevice* device, size_t size) {
		// 結果確認用
		HRESULT result = S_FALSE;

		/// ヒープの設定
		// ヒープ設定用構造体を定義
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};
		// GPUにアップロードする設定
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;

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

		// バッファリソースを生成する
		result = device->GetDevice()->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource_));

		// 生成に失敗していた場合停止させる
		assert(SUCCEEDED(result));
	}

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};
