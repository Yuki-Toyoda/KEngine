#pragma once
/// <summary>
/// 2次元ベクトル構造体
/// </summary>
struct Vector2 final {
	float x; // x軸
	float y; // y軸

	// 演算子オーバーロード(+)
	Vector2 operator+(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x + v.x;
		result.y = y + v.y;

		// 計算結果を返す
		return result;
	}
	// 演算子オーバーロード(+=)
	Vector2 operator+=(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x + v.x;
		result.y = y + v.y;

		// 計算結果を返す
		return result;
	}

	// 演算子オーバーロード(-)
	Vector2 operator-(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x - v.x;
		result.y = y - v.y;

		// 計算結果を返す
		return result;
	}
	// 演算子オーバーロード(-=)
	Vector2 operator-=(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x - v.x;
		result.y = y - v.y;

		// 計算結果を返す
		return result;
	}

	// 演算子オーバーロード(*)
	Vector2 operator*(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x * v.x;
		result.y = y * v.y;

		// 計算結果を返す
		return result;
	}
	// 演算子オーバーロード(*=)
	Vector2 operator*=(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x * v.x;
		result.y = y * v.y;

		// 計算結果を返す
		return result;
	}

	// 演算子オーバーロード(/)
	Vector2 operator/(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x / v.x;
		result.y = y / v.y;

		// 計算結果を返す
		return result;
	}
	// 演算子オーバーロード(/=)
	Vector2 operator/=(const Vector2 v) {
		// 結果格納用
		Vector2 result;

		// 計算処理
		result.x = x / v.x;
		result.y = y / v.y;

		// 計算結果を返す
		return result;
	}
};
