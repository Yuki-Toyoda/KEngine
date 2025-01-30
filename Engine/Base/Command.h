#pragma once
#include "Device/DirectXDevice.h"

#include <cassert>
#include <functional>

/// <summary>
/// コマンド関連総括クラス
/// </summary>
class Command final {
public: // コンストラクタ等

	/// <summary>
	/// コマンド
	/// </summary>
	Command() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Command() = default;

public:// メンバ関数

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">デバイス</param>
	void Init(DirectXDevice* device) {
		// 結果確認用
		HRESULT result = S_FALSE;

		// コマンドキューの生成
		D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};   // 設定用構造体の初期化
		result = device->GetDevice()->CreateCommandQueue(
			&commandQueueDesc, IID_PPV_ARGS(&queue_)); // キューの生成
		assert(SUCCEEDED(result));					   // 生成の成否確認

		// コマンドアロケーターの生成
		result = device->GetDevice()->CreateCommandAllocator(
			D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&allocator_)); // アロケーターの生成
		assert(SUCCEEDED(result));										// 生成の成否確認

		// コマンドリストの生成
		result = device->GetDevice()->CreateCommandList(
			0, D3D12_COMMAND_LIST_TYPE_DIRECT, allocator_.Get(),
			nullptr, IID_PPV_ARGS(&list_));						 // リストの生成
		assert(SUCCEEDED(result));								 // 生成の成否確認

		// 初期値0のフェンスを生成する
		fenceVal_ = 0;												  // 初期値0で設定
		result = device->GetDevice()->CreateFence(
			fenceVal_, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence_)); // フェンスの生成
		assert(SUCCEEDED(result));									  // 生成の成否確認
	}

	/// <summary>
	/// 描画命令実行関数
	/// </summary>
	void Execute() {
		// 結果確認用
		HRESULT result = S_FALSE;

		// コマンドリストを閉じる
		list_->Close();

		// GPUにコマンドリストの実行を指示
		ID3D12CommandList* commandLists[] = { list_.Get() }; // コマンドリスト取得
		queue_->ExecuteCommandLists(1, commandLists);		 // コマンドリスト実行

		// GPUがここまでたどり着いた際、Fenceの値を指定した値に代入するようにSignalを送る
		queue_->Signal(fence_.Get(), ++fenceVal_);
		// FenceのSignal値が指定した値かどうかを確認する
		if (fence_->GetCompletedValue() != fenceVal_) {
			// FenceのSignalを待つためのイベントを生成
			HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			// イベントが生成出来ているか確認
			assert(fenceEvent != nullptr);
			// 指定したSignal値にたどり着いていないため、待機するイベントを設定
			fence_->SetEventOnCompletion(fenceVal_, fenceEvent);
			// イベント終了まで待機
			WaitForSingleObject(fenceEvent, INFINITE);
			// 終了したら閉じる
			CloseHandle(fenceEvent);
		}

		// 次のコマンドリストを用意する
		result = allocator_->Reset();					  // アロケーターリセット
		assert(SUCCEEDED(result));						  // リセット成否確認
		result = list_->Reset(allocator_.Get(), nullptr); // コマンドリストリセット
		assert(SUCCEEDED(result));						  // リセット成否確認

		// コマンドに登録されているリソースを削除する
		ReleaseAllResources();
	}

	/// <summary>
	/// 配列に登録されている全てのリリース破棄関数を呼び出す関数
	/// </summary>
	void ReleaseAllResources() {
		// 全ての登録されているリソースを削除する
		for (const std::function<void()>& func : releaseFunctions_) {
			func();
		}

		// リソース削除後リストをクリアする
		releaseFunctions_.clear();
	}

public: // アクセッサ等

	/// <summary>
	/// コマンドキューゲッター
	/// </summary>
	/// <returns>コマンドキュー</returns>
	ID3D12CommandQueue* Queue() { return queue_.Get(); }

	/// <summary>
	/// アロケーターゲッター
	/// </summary>
	/// <returns>コマンドアロケーター</returns>
	ID3D12CommandAllocator* Allocator() { return allocator_.Get(); }

	/// <summary>
	/// コマンドリストゲッター
	/// </summary>
	/// <returns>コマンドリスト</returns>
	ID3D12GraphicsCommandList6* List() { return list_.Get(); }

	/// <summary>
	/// ポインタをリリースする関数ポインタを追加するアクセッサ
	/// </summary>
	/// <param name="function">追加する関数ポインタ</param>
	void AddReleaseFunction(std::function<void()> func) {
		// 関数ポインタを追加
		releaseFunctions_.push_back(func);
	}

private: // メンバ変数

	// コマンドキュー
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> queue_;
	// アロケーター
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> allocator_;
	// リスト
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList6> list_;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	// テクスチャの中間リソース解放関数格納配列
	std::vector<std::function<void()>> releaseFunctions_;
};
