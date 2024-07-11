#pragma once

/// <summary>
/// カウンタクラス
/// </summary>
class Counter
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Counter() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Counter() = default;

public: // メンバ関数

	/// <summary>
	/// カウンターリセット関数
	/// </summary>
	void Reset() { count_ = 0; }

	/// <summary>
	/// カウント加算
	/// </summary>
	void Increment() { count_++; }

	/// <summary>
	/// カウント減算
	/// </summary>
	void Decrement() { count_--; }

public: // アクセッサ

	/// <summary>
	/// 数値ゲッター
	/// </summary>
	/// <returns>カウンターの数値</returns>
	const int GetCount() { return count_; }

	/// <summary>
	/// カウント取得 + カウント加算関数
	/// </summary>
	/// <returns>カウンターの数値 + 1</returns>
	const int GetCountAndIncrement() { return count_++; }

	/// <summary>
	/// カウント取得 + カウント減算関数
	/// </summary>
	/// <returns>カウンターの数値 - 1</returns>
	const int GetCountAndDecrement() { return count_--; }

private: // メンバ変数

	// 数値
	int count_ = 0;

};