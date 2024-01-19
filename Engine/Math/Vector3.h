#pragma once

/// <summary>
/// 3次元ベクトル構造体
/// </summary>
struct Vector3 final {
	float x; // x軸
	float y; // y軸
	float z; // z軸

	// 演算子オーバーロード(+)
	Vector3 operator+(const Vector3 v) const {
		return { (this->x) + (v.x), (this->y) + (v.y), (this->z) + (v.z)};
	}
	// 演算子オーバーロード(+=)
	void operator+=(const Vector3 v) {
		this->x += v.x;
		this->y += v.y;
		this->z += v.z;
	}

	// 演算子オーバーロード(-)
	Vector3 operator-(const Vector3 v) const {
		return { (this->x) - (v.x), (this->y) - (v.y), (this->z) - (v.z) };
	}

	// 演算子オーバーロード(-=)
	Vector3 operator-=(const Vector3 v) {
		this->x -= v.x;
		this->y -= v.y;
		this->z -= v.z;
		return *this;
	}

	// 演算子オーバーロード(*)
	Vector3 operator*(const Vector3 v) const {
		return { (this->x) * (v.x), (this->y) * (v.y), (this->z) * (v.z) };
	}
	// 演算子オーバーロード(*)
	Vector3 operator*(const float v) const {
		return { (this->x) * (v), (this->y) * (v), (this->z) * (v) };
	}
	// 演算子オーバーロード(*=)
	Vector3 operator*=(const Vector3 v) {
		this->x *= v.x;
		this->y *= v.y;
		this->z *= v.z;
		return *this;
	}

	// 演算子オーバーロード(/)
	Vector3 operator/(const Vector3 v) const {
		return { (this->x) / (v.x), (this->y) / (v.y), (this->z) / (v.z) };
	}
	// 演算子オーバーロード(/=)
	Vector3 operator/=(const Vector3 v) {
		this->x /= v.x;
		this->y /= v.y;
		this->z /= v.z;
		return *this;
	}

	// 演算子オーバーロード(==)
	bool operator==(const Vector3 v) const {
		return { x == v.x && y == v.y && z == v.z };
	}
};