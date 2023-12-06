#pragma once

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
		return start + (end - start) * t;
	}

}
