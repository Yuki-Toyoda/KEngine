#pragma once
#include <vector>
#include "IDescriptorHeap.h"
#include "../Config/Config.h"

// 前方宣言
namespace DirectX {
	class ScratchImage;
}

/// <summary>
/// SRV情報構造体
/// </summary>
struct SRVInfo : public HeapInfo {

	// GPU上のアドレス
	D3D12_GPU_DESCRIPTOR_HANDLE gpuView_{};
	// SRV設定用構造体
	D3D12_SHADER_RESOURCE_VIEW_DESC desc_{};

	// デフォルトコンストラクタ
	SRVInfo() = default;

	/// <summary>
	/// アドレスセッター
	/// </summary>
	/// <param name="heap">アドレスを取得するヒープ</param>
	void SetView(IDescriptorHeap* heap) override {
		// CPU上のアドレス取得
		cpuView_ = heap->GetCPUHandle(index_);
		// GPU上のアドレス取得
		gpuView_ = heap->GetGPUHandle(index_);
	}

	/// <summary>
	/// ムーブコンストラクタ
	/// </summary>
	/// <param name="other">他のSRV情報</param>
	SRVInfo(SRVInfo&& other) noexcept : HeapInfo(std::move(other)) {
		// 引数をメンバ変数に代入
		gpuView_ = std::exchange(other.gpuView_, {});
		desc_	 = std::exchange(other.desc_, {});
	}

	/// <summary>
	/// 代入演算子オーバーロード
	/// </summary>
	/// <param name="other">他のSRV情報</param>
	/// <returns>代入後のデータ</returns>
	SRVInfo& operator=(SRVInfo&& other) noexcept {
		// 代入先と一致していないときのみ代入を行う
		if (this != &other) {
			// 継承元のオーバーロードを使用する
			HeapInfo::operator=(std::move(other));
			// 引数をメンバ変数に代入
			gpuView_ = std::exchange(other.gpuView_, {});
			desc_	 = std::exchange(other.desc_, {});
		}
		// 代入後のデータを返す
		return *this;
	}

	// IndexListのバグを防ぐためコピー操作を禁止する
	SRVInfo(const SRVInfo&) = delete;
	SRVInfo& operator=(const SRVInfo&) = delete;
};

/// <summary>
/// SRVヒープクラス
/// </summary>
class SRV : public IDescriptorHeap
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	SRV() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	SRV(ID3D12Device2* device);

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 構造体バッファをSRVに登録する関数
	/// </summary>
	/// <param name="resource">登録するリソース</param>
	/// <param name="desc">構造体バッファのフォーマット</param>
	/// <returns>構造体バッファまでのアドレス</returns>
	SRVInfo RegisterStructuredBuffer(ID3D12Resource* resource, const D3D12_SHADER_RESOURCE_VIEW_DESC& desc);

	/// <summary>
	/// テクスチャをSRVに登録する関数
	/// </summary>
	/// <param name="resource">登録するリソース</param>
	/// <param name="mipImages">ミップマップ付きテクスチャ</param>
	/// <returns>テクスチャまでのアドレス</returns>
	SRVInfo RegisterTexture(ID3D12Resource* resource, const DirectX::ScratchImage& mipImages);

	/// <summary>
	/// ImGui用のスペースをSRV上に確保する関数
	/// </summary>
	/// <returns>ImGuiの登録情報までのアドレス</returns>
	SRVInfo CreateImGuiSpace();

public: // アクセッサ等

	/// <summary>
	/// 最初のテクスチャのGPU上のアドレスゲッター
	/// </summary>
	/// <returns>最初のテクスチャのGPU上のアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetFirstTexView();

private: // メンバ変数

	// テクスチャ用インデックスリスト
	IndexList texIndexList_;

};

