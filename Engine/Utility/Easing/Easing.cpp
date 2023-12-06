#include "Easing.h"
#include <cmath>
#include <numbers>

float KLib::EaseInQuad(float t)
{
	// イージングされた値を返す
	return t * t;
}

float KLib::EaseOutQuad(float t)
{
	// イージングされた値を返す
	return 1.0f - (1.0f - t) * (1.0f - t);
}

float KLib::EaseInOutQuad(float t)
{
	// イージングされた値を返す
	return t < 0.5f ? 2.0f * t * t : 1.0f - std::pow(-2.0f * t + 2.0f, 2.0f) / 2.0f;
}

float KLib::EaseInSine(float t)
{
	// イージングされた値を返す
	return 1.0f - std::cos((t * (float)std::numbers::pi) / 2.0f);
}

float KLib::EaseOutSine(float t)
{
	// イージングされた値を返す
	return std::sin((t * (float)std::numbers::pi) / 2.0f);
}

float KLib::EaseInOutSine(float t)
{
	// イージングされた値を返す
	return -(std::cos(t * (float)std::numbers::pi) - 1.0f) / 2.0f;
}

float KLib::EaseInBack(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return c3 * t * t * t - c1 * t * t;
}

float KLib::EaseOutBack(float t)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1.0f;

	return 1.0f + c3 * std::pow(t - 1.0f, 3.0f) + c1 * std::pow(t - 1.0f, 2.0f);
}

float KLib::EaseInOutBack(float t)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return t < 0.5f
		? (std::pow(2.0f * t, 2.0f) * ((c2 + 1.0f) * 2.0f * t - c2)) / 2.0f
		: (std::pow(2.0f * t - 2.0f, 2.0f) * ((c2 + 1.0f) * (t * 2.0f - 2.0f) + c2) + 2.0f) / 2.0f;
}

float KLib::EaseInBounce(float t)
{
	return 1.0f - KLib::EaseOutBounce(1.0f - t);
}

float KLib::EaseOutBounce(float t)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (t < (1.0f / d1)) {
		return n1 *  t * t;
	}
	else if (t < (2.0f / d1)) {
		return n1 * (t -= 1.5f / d1) * t + 0.75f;
	}
	else if (t < (2.5f / d1)) {
		return n1 * (t -= 2.25f / d1) * t + 0.9375f;
	}
	else {
		return n1 * (t -= 2.625f / d1) * t + 0.984375f;
	}
}

float KLib::EaseInOutBouce(float t)
{
	return t < 0.5f
		? (1.0f - KLib::EaseOutBounce(1.0f - 2.0f * t)) / 2.0f
		: (1.0f + KLib::EaseOutBounce(2.0f * t - 1.0f)) / 2.0f;
}
