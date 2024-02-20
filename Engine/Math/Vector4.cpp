#include "Vector4.h"

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

// 静的なメンバ関数の定義
const Vector4 Vector4::kZero = { 0.0f, 0.0f, 0.0f, 0.0f };