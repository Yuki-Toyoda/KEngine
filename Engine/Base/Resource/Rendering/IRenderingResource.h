#pragma once
#include "../IResource.h"
#include "../../Config/Config.h"

/// <summary>
/// レンダリング用リソース基底クラス
/// </summary>
class IRenderingResource : public IResource {
public: // 純粋仮想関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="heaps">ヒープマネージャー</param>
	virtual void Init(DirectXDevice* device, HeapManager* heaps) = 0;

	/// <summary>
	/// 画面クリア関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	virtual void Clear(ID3D12GraphicsCommandList6* list) = 0;

public: // アクセッサ等

	/// <summary>
	/// 現在のリソースバリアゲッター
	/// </summary>
	/// <returns>現在のリソースバリア</returns>
	D3D12_RESOURCE_STATES GetBarrier() { return currentBarrierState_; }

	/// <summary>
	/// リソースバリア変更関数
	/// </summary>
	/// <param name="state">変更するバリア</param>
	/// <param name="list">コマンドリスト</param>
	void ChangeResourceBarrier(D3D12_RESOURCE_STATES state, ID3D12GraphicsCommandList6* list) {
		// 変更するバリアが同一の場合、何もせずに処理を抜ける
		if (currentBarrierState_ == state) { return; }

		// TransitionBarrierの設定用構造体を定義
		D3D12_RESOURCE_BARRIER barrier{};
		barrier.Type				   = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION; // バリアの種類を設定
		barrier.Flags				   = D3D12_RESOURCE_BARRIER_FLAG_NONE;		 // フラッグ設定
		barrier.Transition.Subresource = 0xFFFFFFFF;							 // 全てのサブリソースを選択
		barrier.Transition.pResource   = resource_.Get();						 // バリアを張る対象のリソース
		barrier.Transition.StateBefore = currentBarrierState_;					 // 遷移前のResourceState
		barrier.Transition.StateAfter  = state;									 // 遷移後のResourceState

		// リソースバリアを変更
		list->ResourceBarrier(1, &barrier);
		// 現在のステータスを変更
		currentBarrierState_ = state;
	}

public: // パブリックなメンバ変数

	// 画面解像度
	int width_	= KEngine::Config::Window::KWindowWidth;  // 横幅
	int height_ = KEngine::Config::Window::KWindowHeight; // 縦幅

	// リソースの設定用構造体
	D3D12_RESOURCE_DESC desc_{};
	// 画面クリア時の色
	D3D12_CLEAR_VALUE clearColor_{};
	// ヒープの設定
	D3D12_HEAP_PROPERTIES properties_{};

protected: // 継承先メンバ変数

	// 現在のリソースバリア
	D3D12_RESOURCE_STATES currentBarrierState_{};

public: // オペレーターオーバーロード

	// 暗黙変換演算子オーバーロード
	operator ID3D12Resource* () {
		return resource_.Get();
	}

};