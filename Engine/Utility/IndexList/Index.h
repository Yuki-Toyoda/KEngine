#pragma once
#include <functional>

/// <summary>
/// 単体インデックスクラス
/// </summary>
class Index {
public: // コンストラクタ等

	/// <summary>
	/// デフォルトコンストラクタ
	/// </summary>
	Index() {
		// 解放用関数ポインタリセット
		func_ = nullptr;
		// インデックスの初期化
		i_ = -1;
	}

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="f">解放用関数</param>
	/// <param name="value">数値</param>
	Index(std::function<void(int)> f, int value) {
		// 解放用関数ポインタ代入
		func_ = f;
		// インデックスを引数から代入
		i_ = value;
	}

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Index() {
		// 関数ポインタが設定されている場合関数実行
		if (func_) { func_(i_); }
	}

public: // アクセッサ等

	/// <summary>
	/// インデックスゲッター
	/// </summary>
	/// <returns>インデックス</returns>
	int GetIndex()const { return i_; }

public: // オペレーターオーバーロード

	/// <summary>
	/// ムーブコンストラクタ
	/// </summary>
	/// <param name="other">その他</param>
	Index(Index&& other) noexcept {
		// 引数のインデックスから解放用関数を取得し、元のインデックスの関数をnullに
		func_ = std::exchange(other.func_, nullptr);
		// 引数のインデックスからインデックスの値を取得し、元のインデックスをリセット
		i_ = std::exchange(other.i_, -1);
	}

	/// <summary>
	/// Index型をint型に変換するオーバーロード
	/// </summary>
	operator int() const {
		// 変数を返す
		return i_;
	}

	// コピー禁止設定
	Index(const Index&) = delete;
	Index& operator=(const Index&) = delete;

public: // 演算子オーバーロード

	/// <summary>
	/// =演算子オーバーロード(move)
	/// </summary>
	/// <param name="other">代入するインデックスクラス</param>
	/// <returns></returns>
	Index& operator=(Index&& other) noexcept {
		// 同一のインデックスの場合
		if (this != &other) {
			// 引数のインデックスから解放用関数を取得し、元のインデックスの関数をnullに
			func_ = std::exchange(other.func_, nullptr);
			// 引数のインデックスからインデックスの値を取得し、元のインデックスをリセット
			i_ = std::exchange(other.i_, -1);
		}
	}

private: // メンバ変数

	// インデックス
	int i_ = -1;
	// 解放用関数ポインタ
	std::function<void(int)> func_ = nullptr;
};