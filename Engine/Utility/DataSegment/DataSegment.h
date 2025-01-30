#pragma once
#include <cstdint>

/// <summary>
/// テンプレートで指定した型の情報をイテレータとして使えるようにするクラス
/// </summary>
template<typename T>
class DataSegment
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DataSegment() : data_(nullptr), count_(0){};

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="data">データ本体</param>
	/// <param name="count">データ数</param>
	DataSegment(T* data, uint32_t count) : data_(data), count_(count) {};

public: // アクセッサ等

	/// <summary>
	/// データ内の先頭要素ゲッター
	/// </summary>
	/// <returns>データ内先頭要素</returns>
	T* GetFront() { return data_; }
	/// <summary>
	/// データ内の先頭要素ゲッター(定数)
	/// </summary>
	/// <returns>データ内の先頭要素(定数)</returns>
	const T* GetFront() const { return data_; }

	/// <summary>
	/// データ内の最終要素ゲッター
	/// </summary>
	/// <returns>データ内の最終要素</returns>
	T& GetBack() { return *(data_ + count_ - 1); }
	/// <summary>
	/// データ内の最終要素ゲッター(定数)
	/// </summary>
	/// <returns>データ内の最終要素</returns>
	const T& GetBack() const { return *(data_ + count_ - 1); }

	/// <summary>
	/// 保存されているデータ数ゲッター
	/// </summary>
	/// <returns>データ数</returns>
	size_t GetSize() const { return count_; }

	/// <summary>
	/// (イテレータ用) 先頭要素ゲッター
	/// </summary>
	/// <returns>データ内の先頭要素</returns>
	T* begin() { return data_; }
	/// <summary>
	/// (イテレータ用) 終端要素ゲッター
	/// </summary>
	/// <returns>データ内の終端要素</returns>
	T* end() { return data_ + count_; }

	/// <summary>
	/// 指定したインデックスのデータ要素に対して書き込み、読み込みを行う
	/// </summary>
	/// <param name="i">指定したインデックス</param>
	/// <returns>指定されたインデックスのデータ要素</returns>
	T& operator[] (uint32_t i) { return *(data_ + i); }
	/// <summary>
	/// 指定したインデックスのデータ要素の値を読み取る(定数値)
	/// </summary>
	/// <param name="i">指定したインデックス</param>
	/// <returns>指定されたインデックスのデータ要素</returns>
	const T& operator[] (uint32_t i) const { return *(data_ + i); }

private: // メンバ変数

	// データ本体
	T* data_;
	// データ数
	uint32_t count_;

};

template <typename T>
DataSegment<T> MakeSegment(T* data, uint32_t size) { return DataSegment<T>(data, size); }
