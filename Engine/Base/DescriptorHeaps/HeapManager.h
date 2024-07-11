#pragma once
#include "SRV.h"
#include "DSV.h"
#include "RTV.h"

#include <memory>

/// <summary>
/// ヒープ管理用マネージャー
/// </summary>
class HeapManager
{
public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	HeapManager() = delete;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="device">デバイス</param>
	HeapManager(DirectXDevice* device);

	/// <summary>
	/// デストラクタ
	/// </summary>
	~HeapManager() = default;

public: // アクセッサ
	
	/// <summary>
	/// RTVヒープゲッター
	/// </summary>
	/// <returns>RTVヒープ</returns>
	RTV* rtv() { return rtv_.get(); }
	/// <summary>
	/// SRVヒープゲッター
	/// </summary>
	/// <returns>SRVヒープ</returns>
	SRV* srv() { return srv_.get(); }
	/// <summary>
	/// DSVヒープゲッター
	/// </summary>
	/// <returns>DSVヒープ</returns>
	DSV* dsv() { return dsv_.get(); }

private: // プライベートなメンバ関数

	// コピー操作を禁止する
	HeapManager(const HeapManager&) = delete;
	const HeapManager& operator=(const HeapManager&) = delete;

private: // メンバ変数

	// RTVヒープクラス
	std::unique_ptr<RTV> rtv_ = nullptr;
	// SRVヒープクラス
	std::unique_ptr<SRV> srv_ = nullptr;
	// DSVヒープクラス
	std::unique_ptr<DSV> dsv_ = nullptr;

};

