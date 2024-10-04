#pragma once
#include "IDescriptorHeap.h"
#include "Engine/Base/Config/Config.h"

struct RTVInfo : public HeapInfo {

	// RTVの設定用構造体
	D3D12_RENDER_TARGET_VIEW_DESC desc{};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RTVInfo() = default;

	/// <summary>
	/// ムーブコンストラクタ
	/// </summary>
	/// <param name="other">他のRTV情報</param>
	RTVInfo(RTVInfo&& other) noexcept : HeapInfo(std::move(other)) {
		desc = std::exchange(other.desc, {});
	}

	/// <summary>
	/// 代入演算子オーバーロード
	/// </summary>
	/// <param name="other">他のRTV情報</param>
	/// <returns>代入後のデータ</returns>
	RTVInfo& operator=(RTVInfo&& other) noexcept {
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
	RTVInfo(const RTVInfo&) = delete;
	RTVInfo& operator=(const RTVInfo&) = delete;
};

/// <summary>
/// RTVヒープクラス
/// </summary>
class RTV : public IDescriptorHeap
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	RTV() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス本体</param>
	RTV(ID3D12Device2* device);

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// RTV生成、およびヒープへの登録をする関数
	/// </summary>
	/// <param name="resource">登録するリソース</param>
	/// <returns>生成、登録したRTV情報</returns>
	RTVInfo CreateRenderTargetView(ID3D12Resource* resource);

};

