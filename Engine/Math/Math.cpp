#include "Math.h"

float Math::Linear(float t, float start, float end, float time) {
	return (float)((1.0 - t / time) * start + t / time * end);
}

int Math::Linear(float t, int start, int end, float time) {
	return (int)((1.0 - t / time) * (float)start + t / time * (float)end);
}

Vector3 Math::Linear(float t, Vector3 start, Vector3 end, float time) {
	// 結果格納用
	Vector3 result;

	result.x = (float)((1.0 - t / time) * start.x + t / time * end.x);
	result.y = (float)((1.0 - t / time) * start.y + t / time * end.y);
	result.z = (float)((1.0 - t / time) * start.z + t / time * end.z);

	return result;
}

float Math::Linear(float t, float start, float end) {
	return (float)((1.0 - t) * start + t * end);
}

int Math::Linear(float t, int start, int end) {
	return (int)((1.0 - t) * (float)start + t * (float)end);
}

Vector3 Math::Linear(float t, Vector3 start, Vector3 end)
{
	// 結果格納用
	Vector3 result;
	result.x = (float)((1.0 - t) * start.x + t * end.x);
	result.y = (float)((1.0 - t) * start.y + t * end.y);
	result.z = (float)((1.0 - t) * start.z + t * end.z);

	return result;
}

float Math::EaseIn(float t, float start, float end, float time) {
	float easeT = t / time * t / time;
	return (float)((1.0 - easeT) * start + easeT * end);
}

int Math::EaseIn(float t, int start, int end, float time) {
	float easeT = t / time * t / time;
	return (int)((1.0 - easeT * (float)start + easeT * (float)end));
}

Vector3 Math::EaseIn(float t, Vector3 start, Vector3 end, float time) {
	// 結果格納用
	Vector3 result;

	float easeT = t / time * t / time;

	result.x = (float)((1.0 - easeT) * start.x + easeT * end.x);
	result.y = (float)((1.0 - easeT) * start.y + easeT * end.y);
	result.z = (float)((1.0 - easeT) * start.z + easeT * end.z);

	return result;
}

float Math::EaseIn(float t, float start, float end)
{
	float easeT = t * t;
	return (float)((1.0 - easeT) * start + easeT * end);
}

int Math::EaseIn(float t, int start, int end) {
	float easeT = t * t;
	return (int)((1.0 - easeT) * (float)start + easeT * (float)end);
}

float Math::EaseOut(float t, float start, float end, float time) {
	float easeT = 1.0f - powf(1.0f - t / time, 3.0f);
	return (float)((1.0 - easeT) * start + easeT * end);
}

int Math::EaseOut(float t, int start, int end, float time) {
	float easeT = 1.0f - powf(1.0f - t / time, 3.0f);
	return (int)((1.0 - easeT) * (float)start + easeT * (float)end);
}

Vector3 Math::EaseOut(float t, Vector3 start, Vector3 end, float time) {
	// 結果格納用
	Vector3 result;

	float easeT = 1.0f - powf(1.0f - t / time, 3.0f);

	result.x = (float)((1.0 - easeT) * start.x + easeT * end.x);
	result.y = (float)((1.0 - easeT) * start.y + easeT * end.y);
	result.z = (float)((1.0 - easeT) * start.z + easeT * end.z);

	return result;
}

float Math::EaseOut(float t, float start, float end)
{
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (float)((1.0 - easeT) * start + easeT * end);
}

int Math::EaseOut(float t, int start, int end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (int)((1.0 - easeT) * (float)start + easeT * (float)end);
}

float Math::EaseInOut(float t, float start, float end, float time)
{
	float easeT = -(cosf((float)std::numbers::pi * t / time) - 1.0f) / 2.0f;
	return (float)((1.0 - easeT) * start + easeT * end);
}

int Math::EaseInOut(float t, int start, int end, float time)
{
	float easeT = -(cosf((float)std::numbers::pi * t / time) - 1.0f) / 2.0f;
	return (int)((1.0 - easeT) * start + easeT * end);
}

Vector3 Math::EaseInOut(float t, Vector3 start, Vector3 end, float time)
{
	// 結果格納用
	Vector3 result;

	float easeT = -(cosf((float)std::numbers::pi * t / time) - 1.0f) / 2.0f;

	result.x = (float)((1.0 - easeT) * start.x + easeT * end.x);
	result.y = (float)((1.0 - easeT) * start.y + easeT * end.y);
	result.z = (float)((1.0 - easeT) * start.z + easeT * end.z);

	return result;

}

float Math::EaseInOut(float t, float start, float end)
{
	float easeT = -(cosf((float)std::numbers::pi * t) - 1.0f) / 2.0f;
	return (float)((1.0 - easeT) * start + easeT * end);
}

int Math::EaseInOut(float t, int start, int end)
{
	float easeT = -(cosf((float)std::numbers::pi * t) - 1.0f) / 2.0f;
	return (int)((1.0 - easeT) * start + easeT * end);
}

float Math::Clamp(float a, float min, float max)
{
	if (a < min) {
		return min;
	}
	else if (max < a) {
		return max;
	}
	return a;
}

void Math::SetSrand() { srand(static_cast<unsigned int>(time(NULL))); }

int Math::Random(int min, int max) {
	return min +
		static_cast<int>(
			rand() * static_cast<float>(max - min + 1.0f) / static_cast<float>(1.0f + RAND_MAX));
}

float Math::RandomF(float min, float max, int num) {
	int Num = (int)pow(10, num);
	return static_cast<float>(
		Random(static_cast<int>(max * Num), static_cast<int>(min * Num)) *
		(1.0f / static_cast<float>(Num)));
}

float Math::Dot(const Vector3& v1, const Vector3& v2)
{
	// 結果格納用
	float result;

	// 計算処理
	result = (v1.x * v2.x) + (v1.y * v2.y) + (v1.z * v2.z);

	return result;
}

float Math::Length(const Vector3& v)
{
	// 計算処理
	return sqrtf(Dot(v, v));
}

float Math::LerpShortAngle(float a, float b, float t)
{
	// 結果格納用
	float result;

	// 角度差分を求める
	float diff = b - a;

	// 角度差分が-2πを下回る、または上回る時
	if (diff <= (float)(-std::numbers::pi * 2.0f) || diff >= (float)(std::numbers::pi * 2.0f))
		diff = std::fmod(a, b); // 角度差分を補正する

	// -πからπに補正する
	if (diff >= (float)std::numbers::pi) {
		diff -= (float)std::numbers::pi * 2.0f;
	}
	else if (diff <= (float)-std::numbers::pi)
		diff += (float)std::numbers::pi * 2.0f;

	// 求めた角度を元に線形補間を行う
	return result = a + (diff * t);

}

Vector3 Math::Normalize(const Vector3& v)
{
	// 正規化するベクトルの長さを求める
	float length = Length(v);
	// 結果格納用
	Vector3 result;

	// 計算処理
	if (length != 0.0f) {
		result.x = v.x / length;
		result.y = v.y / length;
		result.z = v.z / length;
	}
	else {
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
	}

	return result;
}

Vector3 Math::Cross(const Vector3& v1, const Vector3& v2)
{
	// 結果格納用
	Vector3 result;

	// 計算処理
	result.x = (v1.y * v2.z) - (v1.z * v2.y);
	result.y = (v1.z * v2.x) - (v1.x * v2.z);
	result.z = (v1.x * v2.y) - (v1.y * v2.x);

	// 結果を返す
	return result;
}

Vector3 Math::Transform(const Vector3& v, const Matrix4x4& m)
{
	// 結果格納用
	Vector3 result;

	// 生成処理
	result.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]) + (1.0f * m.m[3][0]);
	result.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]) + (1.0f * m.m[3][1]);
	result.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]) + (1.0f * m.m[3][2]);
	float w = (v.x * m.m[0][3]) + (v.y * m.m[1][3]) + (v.z * m.m[2][3]) + (1.0f * m.m[3][3]);

	assert(w != 0.0f);

	result.x /= w;
	result.y /= w;
	result.z /= w;

	return result;
}

Vector3 Math::TransformNormal(const Vector3& v, const Matrix4x4& m)
{
	// 結果格納用
	Vector3 result;

	// 生成処理
	result.x = (v.x * m.m[0][0]) + (v.y * m.m[1][0]) + (v.z * m.m[2][0]);
	result.y = (v.x * m.m[0][1]) + (v.y * m.m[1][1]) + (v.z * m.m[2][1]);
	result.z = (v.x * m.m[0][2]) + (v.y * m.m[1][2]) + (v.z * m.m[2][2]);

	return result;
}

Matrix4x4 Math::Inverse(const Matrix4x4 m)
{
	// 結果格納用
	Matrix4x4 result = {};

	// 行列式
	float d;

	// 行列式を求める
	d = (m.m[0][0] * m.m[1][1] * m.m[2][2] * m.m[3][3]) +
		(m.m[0][0] * m.m[1][2] * m.m[2][3] * m.m[3][1]) +
		(m.m[0][0] * m.m[1][3] * m.m[2][1] * m.m[3][2]) -

		(m.m[0][0] * m.m[1][3] * m.m[2][2] * m.m[3][1]) -
		(m.m[0][0] * m.m[1][2] * m.m[2][1] * m.m[3][3]) -
		(m.m[0][0] * m.m[1][1] * m.m[2][3] * m.m[3][2]) -

		(m.m[0][1] * m.m[1][0] * m.m[2][2] * m.m[3][3]) -
		(m.m[0][2] * m.m[1][0] * m.m[2][3] * m.m[3][1]) -
		(m.m[0][3] * m.m[1][0] * m.m[2][1] * m.m[3][2]) +

		(m.m[0][3] * m.m[1][0] * m.m[2][2] * m.m[3][1]) +
		(m.m[0][2] * m.m[1][0] * m.m[2][1] * m.m[3][3]) +
		(m.m[0][1] * m.m[1][0] * m.m[2][3] * m.m[3][2]) +

		(m.m[0][1] * m.m[1][2] * m.m[2][0] * m.m[3][3]) +
		(m.m[0][2] * m.m[1][3] * m.m[2][0] * m.m[3][1]) +
		(m.m[0][3] * m.m[1][1] * m.m[2][0] * m.m[3][2]) -

		(m.m[0][3] * m.m[1][2] * m.m[2][0] * m.m[3][1]) -
		(m.m[0][2] * m.m[1][1] * m.m[2][0] * m.m[3][3]) -
		(m.m[0][1] * m.m[1][3] * m.m[2][0] * m.m[3][2]) -

		(m.m[0][1] * m.m[1][2] * m.m[2][3] * m.m[3][0]) -
		(m.m[0][2] * m.m[1][3] * m.m[2][1] * m.m[3][0]) -
		(m.m[0][3] * m.m[1][1] * m.m[2][2] * m.m[3][0]) +

		(m.m[0][3] * m.m[1][2] * m.m[2][1] * m.m[3][0]) +
		(m.m[0][2] * m.m[1][1] * m.m[2][3] * m.m[3][0]) +
		(m.m[0][1] * m.m[1][3] * m.m[2][2] * m.m[3][0]);

	// 計算処理
	if (d != 0) {

		result.m[0][0] = (1.0f / d) *
			(m.m[1][1] * m.m[2][2] * m.m[3][3] + m.m[1][2] * m.m[2][3] * m.m[3][1] + m.m[1][3] * m.m[2][1] * m.m[3][2]
				- m.m[1][3] * m.m[2][2] * m.m[3][1] - m.m[1][2] * m.m[2][1] * m.m[3][3] - m.m[1][1] * m.m[2][3] * m.m[3][2]);
		result.m[0][1] = (1.0f / d) *
			(-m.m[0][1] * m.m[2][2] * m.m[3][3] - m.m[0][2] * m.m[2][3] * m.m[3][1] - m.m[0][3] * m.m[2][1] * m.m[3][2]
				+ m.m[0][3] * m.m[2][2] * m.m[3][1] + m.m[0][2] * m.m[2][1] * m.m[3][3] + m.m[0][1] * m.m[2][3] * m.m[3][2]);
		result.m[0][2] = (1.0f / d) *
			(+m.m[0][1] * m.m[1][2] * m.m[3][3] + m.m[0][2] * m.m[1][3] * m.m[3][1] + m.m[0][3] * m.m[1][1] * m.m[3][2]
				- m.m[0][3] * m.m[1][2] * m.m[3][1] - m.m[0][2] * m.m[1][1] * m.m[3][3] - m.m[0][1] * m.m[1][3] * m.m[3][2]);
		result.m[0][3] = (1.0f / d) *
			(-m.m[0][1] * m.m[1][2] * m.m[2][3] - m.m[0][2] * m.m[1][3] * m.m[2][1] - m.m[0][3] * m.m[1][1] * m.m[2][2]
				+ m.m[0][3] * m.m[1][2] * m.m[2][1] + m.m[0][2] * m.m[1][1] * m.m[2][3] + m.m[0][1] * m.m[1][3] * m.m[2][2]);


		result.m[1][0] = (1.0f / d) *
			(-m.m[1][0] * m.m[2][2] * m.m[3][3] - m.m[1][2] * m.m[2][3] * m.m[3][0] - m.m[1][3] * m.m[2][0] * m.m[3][2]
				+ m.m[1][3] * m.m[2][2] * m.m[3][0] + m.m[1][2] * m.m[2][0] * m.m[3][3] + m.m[1][0] * m.m[2][3] * m.m[3][2]);
		result.m[1][1] = (1.0f / d) *
			(m.m[0][0] * m.m[2][2] * m.m[3][3] + m.m[0][2] * m.m[2][3] * m.m[3][0] + m.m[0][3] * m.m[2][0] * m.m[3][2]
				- m.m[0][3] * m.m[2][2] * m.m[3][0] - m.m[0][2] * m.m[2][0] * m.m[3][3] - m.m[0][0] * m.m[2][3] * m.m[3][2]);
		result.m[1][2] = (1.0f / d) *
			(-m.m[0][0] * m.m[1][2] * m.m[3][3] - m.m[0][2] * m.m[1][3] * m.m[3][0] - m.m[0][3] * m.m[1][0] * m.m[3][2]
				+ m.m[0][3] * m.m[1][2] * m.m[3][0] + m.m[0][2] * m.m[1][0] * m.m[3][3] + m.m[0][0] * m.m[1][3] * m.m[3][2]);
		result.m[1][3] = (1.0f / d) *
			(m.m[0][0] * m.m[1][2] * m.m[2][3] + m.m[0][2] * m.m[1][3] * m.m[2][0] + m.m[0][3] * m.m[1][0] * m.m[2][2]
				- m.m[0][3] * m.m[1][2] * m.m[2][0] - m.m[0][2] * m.m[1][0] * m.m[2][3] - m.m[0][0] * m.m[1][3] * m.m[2][2]);


		result.m[2][0] = (1.0f / d) *
			(m.m[1][0] * m.m[2][1] * m.m[3][3] + m.m[1][1] * m.m[2][3] * m.m[3][0] + m.m[1][3] * m.m[2][0] * m.m[3][1]
				- m.m[1][3] * m.m[2][1] * m.m[3][0] - m.m[1][1] * m.m[2][0] * m.m[3][3] - m.m[1][0] * m.m[2][3] * m.m[3][1]);
		result.m[2][1] = (1.0f / d) *
			(-m.m[0][0] * m.m[2][1] * m.m[3][3] - m.m[0][1] * m.m[2][3] * m.m[3][0] - m.m[0][3] * m.m[2][0] * m.m[3][1]
				+ m.m[0][3] * m.m[2][1] * m.m[3][0] + m.m[0][1] * m.m[2][0] * m.m[3][3] + m.m[0][0] * m.m[2][3] * m.m[3][1]);
		result.m[2][2] = (1.0f / d) *
			(m.m[0][0] * m.m[1][1] * m.m[3][3] + m.m[0][1] * m.m[1][3] * m.m[3][0] + m.m[0][3] * m.m[1][0] * m.m[3][1]
				- m.m[0][3] * m.m[1][1] * m.m[3][0] - m.m[0][1] * m.m[1][0] * m.m[3][3] - m.m[0][0] * m.m[1][3] * m.m[3][1]);
		result.m[2][3] = (1.0f / d) *
			(-m.m[0][0] * m.m[1][1] * m.m[2][3] - m.m[0][1] * m.m[1][3] * m.m[2][0] - m.m[0][3] * m.m[1][0] * m.m[2][1]
				+ m.m[0][3] * m.m[1][1] * m.m[2][0] + m.m[0][1] * m.m[1][0] * m.m[2][3] + m.m[0][0] * m.m[1][3] * m.m[2][1]);


		result.m[3][0] = (1.0f / d) *
			(-m.m[1][0] * m.m[2][1] * m.m[3][2] - m.m[1][1] * m.m[2][2] * m.m[3][0] - m.m[1][2] * m.m[2][0] * m.m[3][1]
				+ m.m[1][2] * m.m[2][1] * m.m[3][0] + m.m[1][1] * m.m[2][0] * m.m[3][2] + m.m[1][0] * m.m[2][2] * m.m[3][1]);
		result.m[3][1] = (1.0f / d) *
			(m.m[0][0] * m.m[2][1] * m.m[3][2] + m.m[0][1] * m.m[2][2] * m.m[3][0] + m.m[0][2] * m.m[2][0] * m.m[3][1]
				- m.m[0][2] * m.m[2][1] * m.m[3][0] - m.m[0][1] * m.m[2][0] * m.m[3][2] - m.m[0][0] * m.m[2][2] * m.m[3][1]);
		result.m[3][2] = (1.0f / d) *
			(-m.m[0][0] * m.m[1][1] * m.m[3][2] - m.m[0][1] * m.m[1][2] * m.m[3][0] - m.m[0][2] * m.m[1][0] * m.m[3][1]
				+ m.m[0][2] * m.m[1][1] * m.m[3][0] + m.m[0][1] * m.m[1][0] * m.m[3][2] + m.m[0][0] * m.m[1][2] * m.m[3][1]);
		result.m[3][3] = (1.0f / d) *
			(m.m[0][0] * m.m[1][1] * m.m[2][2] + m.m[0][1] * m.m[1][2] * m.m[2][0] + m.m[0][2] * m.m[1][0] * m.m[2][1]
				- m.m[0][2] * m.m[1][1] * m.m[2][0] - m.m[0][1] * m.m[1][0] * m.m[2][2] - m.m[0][0] * m.m[1][2] * m.m[2][1]);

	}

	return result;
}

Matrix4x4 Math::MakeIdentity4x4()
{
	// 結果格納用
	Matrix4x4 result{};

	// 生成処理
	result.m[0][0] = 1.0f;
	result.m[0][1] = 0.0f;
	result.m[0][2] = 0.0f;
	result.m[0][3] = 0.0f;

	result.m[1][0] = 0.0f;
	result.m[1][1] = 1.0f;
	result.m[1][2] = 0.0f;
	result.m[1][3] = 0.0f;

	result.m[2][0] = 0.0f;
	result.m[2][1] = 0.0f;
	result.m[2][2] = 1.0f;
	result.m[2][3] = 0.0f;

	result.m[3][0] = 0.0f;
	result.m[3][1] = 0.0f;
	result.m[3][2] = 0.0f;
	result.m[3][3] = 1.0f;

	// 生成したものを返す
	return result;
}

Matrix4x4 Math::MakeTranslateMatrix(const Vector3& translate)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[3][0] = translate.x;
	result.m[3][1] = translate.y;
	result.m[3][2] = translate.z;

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeScaleMatrix(const Vector3& scale)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = scale.x;
	result.m[1][1] = scale.y;
	result.m[2][2] = scale.z;

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeRotateXMatrix(const float& X)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[1][1] = std::cosf(X);
	result.m[1][2] = std::sinf(X);
	result.m[2][1] = -(std::sinf(X));
	result.m[2][2] = std::cosf(X);

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeRotateYMatrix(const float& Y)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = std::cosf(Y);
	result.m[0][2] = -(std::sinf(Y));
	result.m[2][0] = std::sinf(Y);
	result.m[2][2] = std::cosf(Y);

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeRotateZMatrix(const float& Z)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = std::cosf(Z);
	result.m[0][1] = std::sinf(Z);
	result.m[1][0] = -(std::sinf(Z));
	result.m[1][1] = std::cosf(Z);
	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeRotateXYZMatrix(const Vector3& rotate)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	Matrix4x4 tempRotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4 tempRotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4 tempRotateZMatrix = MakeRotateZMatrix(rotate.z);

	result = tempRotateXMatrix * tempRotateYMatrix * tempRotateZMatrix;

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	Matrix4x4 S = MakeScaleMatrix(scale);
	Matrix4x4 R = MakeRotateXYZMatrix(rotate);
	Matrix4x4 T = MakeTranslateMatrix(translate);

	result = S * R * T;

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeAffineMatrix(const Vector3& scale, const Quaternion& rotate, const Vector3& translate)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	Matrix4x4 S = MakeScaleMatrix(scale);
	Matrix4x4 R = QuaternionToMatrix(rotate);
	Matrix4x4 T = MakeTranslateMatrix(translate);

	result = S * R * T;

	// 生成結果を返す
	return result;
}

Matrix4x4 Math::MakeOrthGraphicMatrix(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = 2 / (right - left);
	result.m[1][1] = 2 / (top - bottom);
	result.m[2][2] = 1 / (farClip - nearClip);
	result.m[3][0] = (left + right) / (left - right);
	result.m[3][1] = (top + bottom) / (bottom - top);
	result.m[3][2] = nearClip / (nearClip - farClip);

	return result;
}

Matrix4x4 Math::MakePerspectiveFovMatrix(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = (1 / aspectRatio) * (1 / tanf(fovY / 2));
	result.m[1][1] = (1 / tanf(fovY / 2));
	result.m[2][2] = farClip / (farClip - nearClip);
	result.m[2][3] = 1.0f;
	result.m[3][2] = -(nearClip * farClip) / (farClip - nearClip);
	result.m[3][3] = 0.0f;

	return result;
}

Matrix4x4 Math::MakeViewportMatrix(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	// 結果格納用単位行列を生成
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = width / 2;
	result.m[1][1] = -height / 2;
	result.m[2][2] = maxDepth - minDepth;
	result.m[3][0] = left + (width / 2);
	result.m[3][1] = top + (height / 2);
	result.m[3][2] = minDepth;

	return result;
}

Matrix4x4 Math::MakeRotateAxisAngle(Vector3 axis, float angle)
{
	// 結果格納用単位行列
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = (float)(pow(axis.x, 2) * (1 - cos(angle)) + cos(angle));
	result.m[0][1] = (axis.x * axis.y) * (1 - cos(angle)) + (axis.z * sin(angle));
	result.m[0][2] = (axis.x * axis.z) * (1 - cos(angle)) - (axis.y * sin(angle));

	result.m[1][0] = (axis.x * axis.y) * (1 - cos(angle)) - (axis.z * sin(angle));
	result.m[1][1] = (float)(pow(axis.y, 2) * (1 - cos(angle)) + cos(angle));
	result.m[1][2] = (axis.y * axis.z) * (1 - cos(angle)) + (axis.x * sin(angle));

	result.m[2][0] = (axis.x * axis.z) * (1 - cos(angle)) + (axis.y * sin(angle));
	result.m[2][1] = (axis.y * axis.z) * (1 - cos(angle)) - (axis.x * sin(angle));
	result.m[2][2] = (float)(pow(axis.z, 2) * (1 - cos(angle)) + cos(angle));

	// 結果を返す
	return result;
}

Matrix4x4 Math::DirectionToDirection(const Vector3& from, const Vector3& to)
{
	// 回転軸を求める
	Vector3 axis = Normalize(Cross(from, to));
	// 角度を求める
	float radianCos = Dot(from, to);			 // cos
	float radianSin = Length(Cross(from, to)); // sin

	// 結果格納用
	Matrix4x4 result = MakeIdentity4x4();

	Vector3 invVec = from * -1.0f;
	// 真逆のベクトルの場合は無限に存在するパターンから１つ取り出す（資料の通り）
	if (invVec.x == to.x && invVec.y == to.y && invVec.z == to.z) {
		if (to.x != 0.0f || to.y != 0.0f)
			axis = { from.y, -from.x, 0.0f };
		else if (to.x != 0.0f || to.z != 0.0f)
			axis = { from.z, 0.0f, -from.x };
	}

	// 計算処理
	result.m[0][0] = powf(axis.x, 2) * (1.0f - radianCos) + radianCos;
	result.m[0][1] = (axis.x * axis.y) * (1.0f - radianCos) + (axis.z * radianSin);
	result.m[0][2] = (axis.x * axis.z) * (1.0f - radianCos) - (axis.y * radianSin);

	result.m[1][0] = (axis.x * axis.y) * (1.0f - radianCos) - (axis.z * radianSin);
	result.m[1][1] = powf(axis.y, 2) * (1.0f - radianCos) + radianCos;
	result.m[1][2] = (axis.y * axis.z) * (1 - radianCos) + (axis.x * radianSin);

	result.m[2][0] = (axis.x * axis.z) * (1.0f - radianCos) + (axis.y * radianSin);
	result.m[2][1] = (axis.y * axis.z) * (1.0f - radianCos) - (axis.x * radianSin);
	result.m[2][2] = powf(axis.z, 2) * (1.0f - radianCos) + radianCos;

	// 結果を返す
	return result;
}

void Math::MatrixImGui(Matrix4x4 m, std::string paramName)
{
	if (ImGui::TreeNode(paramName.c_str())) {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				std::string param = std::to_string(m.m[i][j]);
				ImGui::Text(param.c_str());
				ImGui::SameLine();
			}
			ImGui::Spacing();
		}
		ImGui::TreePop();
	}	
}

Quaternion Math::MakeIdentityQuaternion()
{
	// 結果格納用
	Quaternion result;

	// 設定
	result.x = 0.0f;
	result.y = 0.0f;
	result.z = 0.0f;
	result.w = 1.0f;

	// 結果を返す
	return result;
}

Quaternion Math::MakeQuaternion(const Vector3& v)
{
	// 結果格納用
	Quaternion result;

	// 設定
	result.x = v.x;
	result.y = v.y;
	result.z = v.z;
	result.w = 0.0f;

	// 結果を返す
	return result;
}

Quaternion Math::Conjugate(const Quaternion& q)
{
	// 結果格納用
	Quaternion result;
	// 引数を取得
	result = q;

	// 虚部を反転
	result.x *= -1.0f;
	result.y *= -1.0f;
	result.z *= -1.0f;

	// 結果を返す
	return result;
}

float Math::Length(const Quaternion& q)
{
	// 結果格納用
	float result;

	// 計算処理
	result = sqrtf(powf(q.w, 2) + powf(q.x, 2) + powf(q.y, 2) + powf(q.z, 2));

	// 結果を返す
	return result;
}

Quaternion Math::Normalize(const Quaternion& q)
{
	// 正規化するベクトルの長さを求める
	float length = Length(q);
	// 結果格納用
	Quaternion result;

	// 計算処理
	if (length != 0.0f) {
		result.x = q.x / length;
		result.y = q.y / length;
		result.z = q.z / length;
		result.w = q.w / length;
	}
	else {
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
		result.w = 0.0f;
	}

	// 結果を返す
	return result;
}

Quaternion Math::Inverse(const Quaternion& q)
{
	// 結果格納用
	Quaternion result = MakeIdentityQuaternion();

	// 正規化するベクトルの長さを求める
	float length = Length(q);
	length = std::powf(length, 2);
	// 共役クォータニオンを求める
	Quaternion conjugate = Math::Conjugate(q);

	// 計算処理
	if (length != 0.0f) {
		result.x = conjugate.x / length;
		result.y = conjugate.y / length;
		result.z = conjugate.z / length;
		result.w = conjugate.w / length;
	}

	// 結果を返す
	return result;
}


Quaternion Math::MakeRotateAxisAngleQuaternion(const Vector3& v, float angle)
{
	// 結果格納用
	Quaternion result = MakeIdentityQuaternion();

	// 計算処理
	result.w = cos((angle / 2.0f));
	result.x = v.x * sin((angle / 2.0f));
	result.y = v.y * sin((angle / 2.0f));
	result.z = v.z * sin((angle / 2.0f));

	// 結果を返す
	return result;
}

Vector3 Math::RoatateVector(const Vector3& v, const Quaternion& q)
{
	// 引数のベクトルを使用してクォータニオンを生成する
	Quaternion vq = MakeIdentityQuaternion();
	vq.x = v.x;
	vq.y = v.y;
	vq.z = v.z;
	vq.w = 0.0f;

	// 生成したクォータニオンを用いてクォータニオンを回転させる
	Quaternion result = q * vq * Conjugate(q);
	// 計算結果を返す
	return result.vec();
}

Matrix4x4 Math::QuaternionToMatrix(const Quaternion& q)
{
	// 結果格納用
	Matrix4x4 result = MakeIdentity4x4();

	// 計算処理
	result.m[0][0] = powf(q.w, 2) + powf(q.x, 2) - powf(q.y, 2) - powf(q.z, 2);
	result.m[0][1] = 2.0f * (q.x * q.y + q.w * q.z);
	result.m[0][2] = 2.0f * (q.x * q.z - q.w * q.y);

	result.m[1][0] = 2.0f * (q.x * q.y - q.w * q.z);
	result.m[1][1] = powf(q.w, 2) - powf(q.x, 2) + powf(q.y, 2) - powf(q.z, 2);
	result.m[1][2] = 2.0f * (q.y * q.z + q.w * q.x);

	result.m[2][0] = 2.0f * (q.x * q.z + q.w * q.y);
	result.m[2][1] = 2.0f * (q.y * q.z - q.w * q.x);
	result.m[2][2] = powf(q.w, 2) - powf(q.x, 2) - powf(q.y, 2) + powf(q.z, 2);

	// 計算結果を返す
	return result;
}
