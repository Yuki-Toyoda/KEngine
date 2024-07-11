#pragma once
#include <vector>
#include "IDescriptorHeap.h"

/// <summary>
/// DSV情報構造体
/// </summary>
struct DSVInfo : public HeapInfo {

	// DSVの設定用構造体
	D3D12_DEPTH_STENCIL_VIEW_DESC desc{};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DSVInfo() = default;

	/// <summary>
	/// ムーブコンストラクタ
	/// </summary>
	/// <param name="other">他のDSV情報</param>
	DSVInfo(DSVInfo&& other) noexcept : HeapInfo(std::move(other)) {
		// 引数をメンバ変数に代入
		desc = std::exchange(other.desc, {});
	}

	/// <summary>
	/// 代入演算子オーバーロード
	/// </summary>
	/// <param name="other">他のDSV情報</param>
	/// <returns>代入後のデータ</returns>
	DSVInfo& operator=(DSVInfo&& other) noexcept {
		if (this != &other) {
			// 継承元のオーバーロードを使用する
			HeapInfo::operator=(std::move(other));
			// 引数をメンバ変数に代入
			desc = std::exchange(other.desc, {});
		}
		// 代入後のデータを返す
		return *this;
	}

	// IndexListのバグを防ぐためコピー操作を禁止する
	DSVInfo(const DSVInfo&) = delete;
	DSVInfo& operator=(const DSVInfo&) = delete;
};

/// <summary>
/// DSVヒープクラス
/// </summary>
class DSV : public IDescriptorHeap
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	DSV() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス本体</param>
	DSV(ID3D12Device2* device);

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// DSVの生成、およびヒープへの登録をする関数
	/// </summary>
	/// <param name="resource">登録するリソース</param>
	/// <returns>生成、登録したDSV情報</returns>
	DSVInfo CreateDepthStencilView(ID3D12Resource* resource);

};

