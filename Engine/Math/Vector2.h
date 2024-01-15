#pragma once
/// <summary>
/// 2次元ベクトル構造体
/// </summary>
struct Vector2 final {
	float x; // x軸
	float y; // y軸

	// 演算子オーバーロード(+)
	Vector2 operator+(const Vector2 v) {
		return { (this->x) + (v.x), (this->y) + (v.y) };
	}
	// 演算子オーバーロード(+=)
	Vector2 operator+=(const Vector2 v) {
		this->x += v.x;
		this->y += v.y;
		return *this;
	}

	// 演算子オーバーロード(-)
	Vector2 operator-(const Vector2 v) {
		return { (this->x) - (v.x), (this->y) - (v.y) };
	}
	// 演算子オーバーロード(-=)
	Vector2 operator-=(const Vector2 v) {
		this->x -= v.x;
		this->y -= v.y;
		return *this;
	}

	// 演算子オーバーロード(*)
	Vector2 operator*(const Vector2 v) {
		return { (this->x) * (v.x), (this->y) * (v.y) };
	}
	// 演算子オーバーロード(*)
	Vector2 operator*(const float v) const {
		return { (this->x) * (v), (this->y) * (v) };
	}
	// 演算子オーバーロード(*=)
	Vector2 operator*=(const Vector2 v) {
		this->x *= v.x;
		this->y *= v.y;
		return *this;
	}

	// 演算子オーバーロード(/)
	Vector2 operator/(const Vector2 v) {
		return { (this->x) / (v.x), (this->y) / (v.y) };
	}
	// 演算子オーバーロード(/=)
	Vector2 operator/=(const Vector2 v) {
		this->x /= v.x;
		this->y /= v.y;
		return *this;
	}

	// 演算子オーバーロード(==)
	bool operator==(const Vector2 v) const {
		return { x == v.x && y == v.y };
	}
};
