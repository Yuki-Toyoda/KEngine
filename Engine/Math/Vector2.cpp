#include "Vector2.h"
#include <cmath>

Vector2::Vector2() noexcept
{
	// 値の初期化
	x = 0.0f;
	y = 0.0f;
}

Vector2::Vector2(float x, float y) noexcept
{
	// 値の代入
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2& v) noexcept
{
	// 値の代入
	*this = v;
}

Vector2 Vector2::operator+(const Vector2& v) const noexcept
{
	// 結果格納用
	Vector2 result;

	// 値の計算を行う
	result.x = this->x + v.x;
	result.y = this->y + v.y;

	// 計算後の値を返す
	return result;
}

Vector2 Vector2::operator-(const Vector2& v) const noexcept
{
	// 結果格納用
	Vector2 result;

	// 値の計算を行う
	result.x = this->x - v.x;
	result.y = this->y - v.y;

	// 計算後の値を返す
	return result;
}

Vector2 Vector2::operator*(const Vector2& v) const noexcept
{
	// 結果格納用
	Vector2 result;

	// 値の計算を行う
	result.x = this->x * v.x;
	result.y = this->y * v.y;

	// 計算後の値を返す
	return result;
}

Vector2 Vector2::operator/(const Vector2& v) const noexcept
{
	// 結果格納用
	Vector2 result;

	// 値の計算を行う
	result.x = this->x / v.x;
	result.y = this->y / v.y;

	// 計算後の値を返す
	return result;
}

Vector2& Vector2::operator=(const Vector2& v) noexcept
{
	// 値の代入を行う
	this->x = v.x;
	this->y = v.y;

	// 代入した値を返還する
	return *this;
}

Vector2& Vector2::operator=(Vector2&& v) noexcept
{
	// 値の代入を行う
	this->x = std::move(v.x);
	this->y = std::move(v.y);

	// 代入した値を返還する
	return *this;
}

Vector2 Vector2::operator+=(const Vector2& v) noexcept
{
	// 計算を行う
	this->x += v.x;
	this->y += v.y;

	// 計算後の値を返還する
	return *this;
}

Vector2 Vector2::operator-=(const Vector2& v) noexcept
{
	// 計算を行う
	this->x -= v.x;
	this->y -= v.y;

	// 計算後の値を返還する
	return *this;
}

Vector2 Vector2::operator*=(const Vector2& v) noexcept
{
	// 計算を行う
	this->x *= v.x;
	this->y *= v.y;

	// 計算後の値を返還する
	return *this;
}

Vector2 Vector2::operator/=(const Vector2& v) noexcept
{
	// 計算を行う
	this->x /= v.x;
	this->y /= v.y;

	// 計算後の値を返還する
	return *this;
}

Vector2 Vector2::operator*(const float& f) const noexcept
{
	// 結果格納用
	Vector2 result;

	// 値の計算を行う
	result.x = this->x * f;
	result.y = this->y * f;

	// 計算後の値を返す
	return result;
}

Vector2 Vector2::operator/(const float& f) const noexcept
{
	// 結果格納用
	Vector2 result;

	// 値の計算を行う
	result.x = this->x / f;
	result.y = this->y / f;

	// 計算後の値を返す
	return result;
}

Vector2 Vector2::operator*=(const float& f) noexcept
{
	// 計算を行う
	this->x *= f;
	this->y *= f;

	// 計算後の値を返還する
	return *this;
}

Vector2 Vector2::operator/=(const float& f) noexcept
{
	// 計算を行う
	this->x /= f;
	this->y /= f;

	// 計算後の値を返還する
	return *this;
}

bool Vector2::operator==(const Vector2& v) const noexcept
{
	// 値が等しいか調べる
	return this->x == v.x && this->y == v.y;
}

bool Vector2::operator!=(const Vector2& v) const noexcept
{
	// 値の一致を調べる
	return this->x != v.x && this->y != v.y;
}

float Vector2::Dot(const Vector2& v) const noexcept
{
	// 内積を求める
	return (x * v.x) + (y * v.y);
}

float Vector2::Length() const noexcept
{
	// ベクトル長さを求める
	return std::hypot(x, y);
}

float Vector2::Cross(const Vector2& v) const noexcept
{
	// クロス積を求める
	return (x * v.x) - (y * v.y);
}

Vector2 Vector2::Normalize() const noexcept
{
	// ゼロベクトルだった場合そのまま返す
	if (*this == Vector2::kZero) {
		return *this;
	}

	// 計算を行う
	return *this / Length();
}


// 静的なメンバ関数の定義
const Vector2 Vector2::kZero = { 0.0f, 0.0f };
