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
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;

		// 計算結果の返却
		return result;
	}
	// 演算子オーバーロード(+=)
	Vector3 operator+=(const Vector3 v) const {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x + v.x;
		result.y = y + v.y;
		result.z = z + v.z;

		// 計算結果の返却
		return result;
	}

	// 演算子オーバーロード(-)
	Vector3 operator-(const Vector3 v) const {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;

		// 計算結果の返却
		return result;
	}

	// 演算子オーバーロード(-=)
	Vector3 operator-=(const Vector3 v) const {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x - v.x;
		result.y = y - v.y;
		result.z = z - v.z;

		// 計算結果の返却
		return result;
	}

	// 演算子オーバーロード(*)
	Vector3 operator*(const Vector3 v) const {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x * v.x;
		result.y = y * v.y;
		result.z = z * v.z;

		// 計算結果の返却
		return result;
	}
	// 演算子オーバーロード(*)
	Vector3 operator*(const float v) const {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x * v;
		result.y = y * v;
		result.z = z * v;

		// 計算結果の返却
		return result;
	}
	// 演算子オーバーロード(*=)
	Vector3 operator*=(const Vector3 v) const {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x * v.x;
		result.y = y * v.y;
		result.z = z * v.z;

		// 計算結果の返却
		return result;
	}

	// 演算子オーバーロード(/)
	Vector3 operator/(const Vector3 v) {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x / v.x;
		result.y = y / v.y;
		result.z = z / v.z;

		// 計算結果を返す
		return result;
	}
	// 演算子オーバーロード(/=)
	Vector3 operator/=(const Vector3 v) {
		// 結果格納用
		Vector3 result;

		// 計算処理
		result.x = x / v.x;
		result.y = y / v.y;
		result.z = z / v.z;

		// 計算結果を返す
		return result;
	}
};