#pragma once
/// <summary>
/// KEngineのライブラリ関数
/// </summary>
namespace KLib {

	/// <summary>
	/// イージングパラメーター列挙子
	/// </summary>
	enum EasingType {
		ParamEaseLinear,
		ParamEaseInQuad,
		ParamEaseOutQuad,
		ParamEaseInOutQuad,
		ParamEaseInSine,
		ParamEaseOutSine,
		ParamEaseInOutSine,
		ParamEaseInBack,
		ParamEaseOutBack,
		ParamEaseInOutBack,
		ParamEaseInBounce,
		ParamEaseOutBounce,
		ParamEaseInOutBounce
	};

	/// <summary>
	/// Linear関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	inline float EaseLinear(float t) { return t; }

	/// <summary>
	/// EaseInQuad関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInQuad(float t);
	/// <summary>
	/// EaseOutQuad関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseOutQuad(float t);
	/// <summary>
	/// EaseInOutQuad関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInOutQuad(float t);

	/// <summary>
	/// EaseInSine関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInSine(float t);
	/// <summary>
	/// EaseOutSine関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseOutSine(float t);
	/// <summary>
	/// EaseInOutSine関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInOutSine(float t);

	/// <summary>
	/// EaseInBack関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInBack(float t);
	/// <summary>
	/// EaseOutBack関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseOutBack(float t);
	/// <summary>
	/// EaseInOutBack関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInOutBack(float t);

	/// <summary>
	/// EaseInBounce関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInBounce(float t);
	/// <summary>
	/// EaseOutBounce関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseOutBounce(float t);
	/// <summary>
	/// EaseInOutBounce関数
	/// </summary>
	/// <param name="t">現在のt(0.0 ~ 1.0)</param>
	/// <returns>イージングされた値</returns>
	float EaseInOutBounce(float t);

}