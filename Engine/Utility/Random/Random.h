#pragma once
#include <cmath>

/// <summary>
/// ランダムクラス
/// </summary>
namespace KLib {

	// ランダム関数（int型）
	// 返り値：ランダムの値（int型）
	// 引数：
	// min ... 最小値
	// max ... 最大値
	// 指定された範囲内のランダムな値を返す関数(int型)
	inline int Random(int min, int max) {
		return min +
			static_cast<int>(
				rand() * static_cast<float>(max - min + 1.0f) / static_cast<float>(1.0f + RAND_MAX));
	}

	// ランダム関数（float型）
	// 返り値：ランダムの値（float型）
	// 引数：
	// min ... 最小値
	// max ... 最大値
	// num ... 少数点何桁まで表示するか
	// 指定された桁数で、範囲内のランダムな値を返す関数(float型)
	inline float RandomF(float min, float max, int num) {
		int Num = (int)pow(10, num);
		return static_cast<float>(
			Random(static_cast<int>(max * Num), static_cast<int>(min * Num)) *
			(1.0f / static_cast<float>(Num)));
	}
}
