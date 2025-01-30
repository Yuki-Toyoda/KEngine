#include "Vector4.h"
#include <cmath>

Vector4::Vector4() noexcept
{
	// 値の初期化
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

Vector4::Vector4(float x, float y, float z, float w) noexcept
{
	// 値の代入
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

Vector4::Vector4(const Vector4& v) noexcept
{
	// 値の代入
	*this = v;
}

Vector4 Vector4::operator+(const Vector4& v) const noexcept
{
	// 結果格納用
	Vector4 result;

	// 値の計算を行う
	result.x = this->x + v.x;
	result.y = this->y + v.y;
	result.z = this->z + v.z;
	result.w = this->w + v.w;

	// 計算後の値を返す
	return result;
}

Vector4 Vector4::operator-(const Vector4& v) const noexcept
{
	// 結果格納用
	Vector4 result;

	// 値の計算を行う
	result.x = this->x - v.x;
	result.y = this->y - v.y;
	result.z = this->z - v.z;
	result.w = this->w - v.w;

	// 計算後の値を返す
	return result;
}

Vector4 Vector4::operator*(const Vector4& v) const noexcept
{
	// 結果格納用
	Vector4 result;

	// 値の計算を行う
	result.x = this->x * v.x;
	result.y = this->y * v.y;
	result.z = this->z * v.z;
	result.w = this->w * v.w;

	// 計算後の値を返す
	return result;
}

Vector4 Vector4::operator/(const Vector4& v) const noexcept
{
	// 結果格納用
	Vector4 result;

	// 値の計算を行う
	result.x = this->x / v.x;
	result.y = this->y / v.y;
	result.z = this->z / v.z;
	result.w = this->w / v.w;

	// 計算後の値を返す
	return result;
}

Vector4& Vector4::operator=(const Vector4& v) noexcept
{
	// 値の代入を行う
	this->x = v.x;
	this->y = v.y;
	this->z = v.z;
	this->w = v.w;

	// 代入した値を返還する
	return *this;
}

Vector4& Vector4::operator=(Vector4&& v) noexcept
{
	// 値の代入を行う
	this->x = std::move(v.x);
	this->y = std::move(v.y);
	this->z = std::move(v.z);
	this->w = std::move(v.w);

	// 代入した値を返還する
	return *this;
}

Vector4 Vector4::operator+=(const Vector4& v) noexcept
{
	// 計算を行う
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;

	// 計算後の値を返還する
	return *this;
}

Vector4 Vector4::operator-=(const Vector4& v) noexcept
{
	// 計算を行う
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;

	// 計算後の値を返還する
	return *this;
}

Vector4 Vector4::operator*=(const Vector4& v) noexcept
{
	// 計算を行う
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
	this->w *= v.w;

	// 計算後の値を返還する
	return *this;
}

Vector4 Vector4::operator/=(const Vector4& v) noexcept
{
	// 計算を行う
	this->x /= v.x;
	this->y /= v.y;
	this->z /= v.z;
	this->w /= v.w;

	// 計算後の値を返還する
	return *this;
}

Vector4 Vector4::operator*(const float& f) const noexcept
{
	// 結果格納用
	Vector4 result;

	// 値の計算を行う
	result.x = this->x * f;
	result.y = this->y * f;
	result.z = this->z * f;
	result.w = this->w * f;

	// 計算後の値を返す
	return result;
}

Vector4 Vector4::operator/(const float& f) const noexcept
{
	// 結果格納用
	Vector4 result;

	// 値の計算を行う
	result.x = this->x / f;
	result.y = this->y / f;
	result.z = this->z / f;
	result.w = this->w / f;

	// 計算後の値を返す
	return result;
}

Vector4 Vector4::operator*=(const float& f) noexcept
{
	// 計算を行う
	this->x *= f;
	this->y *= f;
	this->z *= f;
	this->w *= f;

	// 計算後の値を返還する
	return *this;
}

Vector4 Vector4::operator/=(const float& f) noexcept
{
	// 計算を行う
	this->x /= f;
	this->y /= f;
	this->z /= f;
	this->w /= f;

	// 計算後の値を返還する
	return *this;
}

bool Vector4::operator==(const Vector4& v) const noexcept
{
	// 値が等しいか調べる
	return this->x == v.x && this->y == v.y && this->z == v.z && this->w == v.w;
}

bool Vector4::operator!=(const Vector4& v) const noexcept
{
	// 値の一致を調べる
	return this->x != v.x && this->y != v.y && this->z != v.z && this->w != v.w;
}


// 静的なメンバ関数の定義
const Vector4 Vector4::kZero = { 0.0f, 0.0f, 0.0f, 0.0f };