#pragma once
/// <summary>
/// 4x4行列構造体
/// </summary>
struct Matrix4x4 final {
	float m[4][4]; // 行列要素

	// 演算子オーバーロード(+)
	Matrix4x4 operator+(const Matrix4x4 matrix) const {
		// 結果格納用
		Matrix4x4 result;

		// 計算処理
		result.m[0][0] = m[0][0] + matrix.m[0][0];
		result.m[1][0] = m[1][0] + matrix.m[1][0];
		result.m[2][0] = m[2][0] + matrix.m[2][0];
		result.m[3][0] = m[3][0] + matrix.m[3][0];

		result.m[0][1] = m[0][1] + matrix.m[0][1];
		result.m[1][1] = m[1][1] + matrix.m[1][1];
		result.m[2][1] = m[2][1] + matrix.m[2][1];
		result.m[3][1] = m[3][1] + matrix.m[3][1];

		result.m[0][2] = m[0][2] + matrix.m[0][2];
		result.m[1][2] = m[1][2] + matrix.m[1][2];
		result.m[2][2] = m[2][2] + matrix.m[2][2];
		result.m[3][2] = m[3][2] + matrix.m[3][2];

		result.m[0][3] = m[0][3] + matrix.m[0][3];
		result.m[1][3] = m[1][3] + matrix.m[1][3];
		result.m[2][3] = m[2][3] + matrix.m[2][3];
		result.m[3][3] = m[3][3] + matrix.m[3][3];

		return result;
	}
	// 演算子オーバーロード(+=)
	Matrix4x4 operator+=(const Matrix4x4 matrix) const {
		// 結果格納用
		Matrix4x4 result;

		// 計算処理
		result.m[0][0] = m[0][0] + matrix.m[0][0];
		result.m[1][0] = m[1][0] + matrix.m[1][0];
		result.m[2][0] = m[2][0] + matrix.m[2][0];
		result.m[3][0] = m[3][0] + matrix.m[3][0];

		result.m[0][1] = m[0][1] + matrix.m[0][1];
		result.m[1][1] = m[1][1] + matrix.m[1][1];
		result.m[2][1] = m[2][1] + matrix.m[2][1];
		result.m[3][1] = m[3][1] + matrix.m[3][1];

		result.m[0][2] = m[0][2] + matrix.m[0][2];
		result.m[1][2] = m[1][2] + matrix.m[1][2];
		result.m[2][2] = m[2][2] + matrix.m[2][2];
		result.m[3][2] = m[3][2] + matrix.m[3][2];

		result.m[0][3] = m[0][3] + matrix.m[0][3];
		result.m[1][3] = m[1][3] + matrix.m[1][3];
		result.m[2][3] = m[2][3] + matrix.m[2][3];
		result.m[3][3] = m[3][3] + matrix.m[3][3];

		return result;
	}

	// 演算子オーバーロード(-)
	Matrix4x4 operator-(const Matrix4x4 matrix) const {
		// 結果格納用
		Matrix4x4 result;

		// 計算処理
		result.m[0][0] = m[0][0] - matrix.m[0][0];
		result.m[1][0] = m[1][0] - matrix.m[1][0];
		result.m[2][0] = m[2][0] - matrix.m[2][0];
		result.m[3][0] = m[3][0] - matrix.m[3][0];

		result.m[0][1] = m[0][1] - matrix.m[0][1];
		result.m[1][1] = m[1][1] - matrix.m[1][1];
		result.m[2][1] = m[2][1] - matrix.m[2][1];
		result.m[3][1] = m[3][1] - matrix.m[3][1];

		result.m[0][2] = m[0][2] - matrix.m[0][2];
		result.m[1][2] = m[1][2] - matrix.m[1][2];
		result.m[2][2] = m[2][2] - matrix.m[2][2];
		result.m[3][2] = m[3][2] - matrix.m[3][2];

		result.m[0][3] = m[0][3] - matrix.m[0][3];
		result.m[1][3] = m[1][3] - matrix.m[1][3];
		result.m[2][3] = m[2][3] - matrix.m[2][3];
		result.m[3][3] = m[3][3] - matrix.m[3][3];

		return result;
	}
	// 演算子オーバーロード(-=)
	Matrix4x4 operator-=(const Matrix4x4 matrix) const {
		// 結果格納用
		Matrix4x4 result;

		// 計算処理
		result.m[0][0] = m[0][0] - matrix.m[0][0];
		result.m[1][0] = m[1][0] - matrix.m[1][0];
		result.m[2][0] = m[2][0] - matrix.m[2][0];
		result.m[3][0] = m[3][0] - matrix.m[3][0];

		result.m[0][1] = m[0][1] - matrix.m[0][1];
		result.m[1][1] = m[1][1] - matrix.m[1][1];
		result.m[2][1] = m[2][1] - matrix.m[2][1];
		result.m[3][1] = m[3][1] - matrix.m[3][1];

		result.m[0][2] = m[0][2] - matrix.m[0][2];
		result.m[1][2] = m[1][2] - matrix.m[1][2];
		result.m[2][2] = m[2][2] - matrix.m[2][2];
		result.m[3][2] = m[3][2] - matrix.m[3][2];

		result.m[0][3] = m[0][3] - matrix.m[0][3];
		result.m[1][3] = m[1][3] - matrix.m[1][3];
		result.m[2][3] = m[2][3] - matrix.m[2][3];
		result.m[3][3] = m[3][3] - matrix.m[3][3];

		return result;
	}

	// 演算子オーバーロード(*)
	Matrix4x4 operator*(const Matrix4x4 matrix) const {
		// 結果格納用
		Matrix4x4 result;

		// 計算処理
		result.m[0][0] = (m[0][0] * matrix.m[0][0]) + (m[0][1] * matrix.m[1][0]) + (m[0][2] * matrix.m[2][0]) + (m[0][3] * matrix.m[3][0]);
		result.m[1][0] = (m[1][0] * matrix.m[0][0]) + (m[1][1] * matrix.m[1][0]) + (m[1][2] * matrix.m[2][0]) + (m[1][3] * matrix.m[3][0]);
		result.m[2][0] = (m[2][0] * matrix.m[0][0]) + (m[2][1] * matrix.m[1][0]) + (m[2][2] * matrix.m[2][0]) + (m[2][3] * matrix.m[3][0]);
		result.m[3][0] = (m[3][0] * matrix.m[0][0]) + (m[3][1] * matrix.m[1][0]) + (m[3][2] * matrix.m[2][0]) + (m[3][3] * matrix.m[3][0]);

		result.m[0][1] = (m[0][0] * matrix.m[0][1]) + (m[0][1] * matrix.m[1][1]) + (m[0][2] * matrix.m[2][1]) + (m[0][3] * matrix.m[3][1]);
		result.m[1][1] = (m[1][0] * matrix.m[0][1]) + (m[1][1] * matrix.m[1][1]) + (m[1][2] * matrix.m[2][1]) + (m[1][3] * matrix.m[3][1]);
		result.m[2][1] = (m[2][0] * matrix.m[0][1]) + (m[2][1] * matrix.m[1][1]) + (m[2][2] * matrix.m[2][1]) + (m[2][3] * matrix.m[3][1]);
		result.m[3][1] = (m[3][0] * matrix.m[0][1]) + (m[3][1] * matrix.m[1][1]) + (m[3][2] * matrix.m[2][1]) + (m[3][3] * matrix.m[3][1]);

		result.m[0][2] = (m[0][0] * matrix.m[0][2]) + (m[0][1] * matrix.m[1][2]) + (m[0][2] * matrix.m[2][2]) + (m[0][3] * matrix.m[3][2]);
		result.m[1][2] = (m[1][0] * matrix.m[0][2]) + (m[1][1] * matrix.m[1][2]) + (m[1][2] * matrix.m[2][2]) + (m[1][3] * matrix.m[3][2]);
		result.m[2][2] = (m[2][0] * matrix.m[0][2]) + (m[2][1] * matrix.m[1][2]) + (m[2][2] * matrix.m[2][2]) + (m[2][3] * matrix.m[3][2]);
		result.m[3][2] = (m[3][0] * matrix.m[0][2]) + (m[3][1] * matrix.m[1][2]) + (m[3][2] * matrix.m[2][2]) + (m[3][3] * matrix.m[3][2]);

		result.m[0][3] = (m[0][0] * matrix.m[0][3]) + (m[0][1] * matrix.m[1][3]) + (m[0][2] * matrix.m[2][3]) + (m[0][3] * matrix.m[3][3]);
		result.m[1][3] = (m[1][0] * matrix.m[0][3]) + (m[1][1] * matrix.m[1][3]) + (m[1][2] * matrix.m[2][3]) + (m[1][3] * matrix.m[3][3]);
		result.m[2][3] = (m[2][0] * matrix.m[0][3]) + (m[2][1] * matrix.m[1][3]) + (m[2][2] * matrix.m[2][3]) + (m[2][3] * matrix.m[3][3]);
		result.m[3][3] = (m[3][0] * matrix.m[0][3]) + (m[3][1] * matrix.m[1][3]) + (m[3][2] * matrix.m[2][3]) + (m[3][3] * matrix.m[3][3]);

		return result;
	}
	// 演算子オーバーロード(*=)
	Matrix4x4 operator*=(const Matrix4x4 matrix) const {
		// 結果格納用
		Matrix4x4 result;

		// 計算処理
		result.m[0][0] = (m[0][0] * matrix.m[0][0]) + (m[0][1] * matrix.m[1][0]) + (m[0][2] * matrix.m[2][0]) + (m[0][3] * matrix.m[3][0]);
		result.m[1][0] = (m[1][0] * matrix.m[0][0]) + (m[1][1] * matrix.m[1][0]) + (m[1][2] * matrix.m[2][0]) + (m[1][3] * matrix.m[3][0]);
		result.m[2][0] = (m[2][0] * matrix.m[0][0]) + (m[2][1] * matrix.m[1][0]) + (m[2][2] * matrix.m[2][0]) + (m[2][3] * matrix.m[3][0]);
		result.m[3][0] = (m[3][0] * matrix.m[0][0]) + (m[3][1] * matrix.m[1][0]) + (m[3][2] * matrix.m[2][0]) + (m[3][3] * matrix.m[3][0]);

		result.m[0][1] = (m[0][0] * matrix.m[0][1]) + (m[0][1] * matrix.m[1][1]) + (m[0][2] * matrix.m[2][1]) + (m[0][3] * matrix.m[3][1]);
		result.m[1][1] = (m[1][0] * matrix.m[0][1]) + (m[1][1] * matrix.m[1][1]) + (m[1][2] * matrix.m[2][1]) + (m[1][3] * matrix.m[3][1]);
		result.m[2][1] = (m[2][0] * matrix.m[0][1]) + (m[2][1] * matrix.m[1][1]) + (m[2][2] * matrix.m[2][1]) + (m[2][3] * matrix.m[3][1]);
		result.m[3][1] = (m[3][0] * matrix.m[0][1]) + (m[3][1] * matrix.m[1][1]) + (m[3][2] * matrix.m[2][1]) + (m[3][3] * matrix.m[3][1]);

		result.m[0][2] = (m[0][0] * matrix.m[0][2]) + (m[0][1] * matrix.m[1][2]) + (m[0][2] * matrix.m[2][2]) + (m[0][3] * matrix.m[3][2]);
		result.m[1][2] = (m[1][0] * matrix.m[0][2]) + (m[1][1] * matrix.m[1][2]) + (m[1][2] * matrix.m[2][2]) + (m[1][3] * matrix.m[3][2]);
		result.m[2][2] = (m[2][0] * matrix.m[0][2]) + (m[2][1] * matrix.m[1][2]) + (m[2][2] * matrix.m[2][2]) + (m[2][3] * matrix.m[3][2]);
		result.m[3][2] = (m[3][0] * matrix.m[0][2]) + (m[3][1] * matrix.m[1][2]) + (m[3][2] * matrix.m[2][2]) + (m[3][3] * matrix.m[3][2]);

		result.m[0][3] = (m[0][0] * matrix.m[0][3]) + (m[0][1] * matrix.m[1][3]) + (m[0][2] * matrix.m[2][3]) + (m[0][3] * matrix.m[3][3]);
		result.m[1][3] = (m[1][0] * matrix.m[0][3]) + (m[1][1] * matrix.m[1][3]) + (m[1][2] * matrix.m[2][3]) + (m[1][3] * matrix.m[3][3]);
		result.m[2][3] = (m[2][0] * matrix.m[0][3]) + (m[2][1] * matrix.m[1][3]) + (m[2][2] * matrix.m[2][3]) + (m[2][3] * matrix.m[3][3]);
		result.m[3][3] = (m[3][0] * matrix.m[0][3]) + (m[3][1] * matrix.m[1][3]) + (m[3][2] * matrix.m[2][3]) + (m[3][3] * matrix.m[3][3]);

		return result;
	}
};
