#pragma once
#include <numbers>
#include <cmath>

/// <summary>
/// KEngineのライブラリ関数
/// </summary>
namespace KLib {

	/// <summary>
	/// Degree角をRadianに変換する関数
	/// </summary>
	/// <param name="degree">変換するdegree角</param>
	/// <returns>radian角</returns>
	const float degreeToRadian(const float degree) {
		return degree * (std::numbers::pi_v<float> / 180.0f);
	}

}