#pragma once
#include <cmath>
#include <numbers>

/// <summary>
/// KEngineのライブラリ関数
/// </summary>
namespace KLib {

	/// <summary>
	/// 線形補間関数
	/// </summary>
	/// <typeparam name="T">任意の型</typeparam>
	/// <param name="start">始端</param>
	/// <param name="end">終端</param>
	/// <param name="t">現在のt (0 ~ 1)</param>
	/// <returns>線形補間後の値</returns>
	template<typename T>
	inline T Lerp(const T& start, const T& end, float t) {
		// 線形補間した値を返す
		return static_cast<T>(start * (1.0f - t) + end * t);
	}

	/// <summary>
	/// 線形補間関数
	/// </summary>
	/// <typeparam name="T">任意の型</typeparam>
	/// <param name="start">始端</param>
	/// <param name="end">終端</param>
	/// <param name="t">現在のt (0 ~ 1)</param>
	/// <param name="time">秒数</param>
	/// <returns>線形補間後の値</returns>
	template<typename T>
	inline T Lerp(const T& start, const T& end, float t, float time) {
		// 線形補間した値を返す
		return static_cast<T>(start * (1.0f - t / time) + end * t / time);
	}

	/// <summary>
	/// 最短角度補間関数
	/// </summary>
	/// <param name="a">角度a</param>
	/// <param name="b">角度b</param>
	/// <param name="t">補間割合(0 ~ 1)</param>
	/// <returns>補完された角度</returns>
	inline float LerpShortAngle(const float& a, const float& b, const float& t) {
		// 結果格納用
		float result;

		// 角度差分を求める
		float diff = b - a;

		// 角度差分が-2πを下回る、または上回る時
		if (diff <= (float)(-std::numbers::pi * 2.0f) || diff >= (float)(std::numbers::pi * 2.0f))
			diff = static_cast<float>(std::fmod(a, b)); // 角度差分を補正する

		// -πからπに補正する
		if (diff >= (float)std::numbers::pi) {
			diff -= (float)std::numbers::pi * 2.0f;
		}
		else if (diff <= (float)-std::numbers::pi)
			diff += (float)std::numbers::pi * 2.0f;

		// 求めた角度を元に線形補間を行う
		return result = a + (diff * t);
	}

}
