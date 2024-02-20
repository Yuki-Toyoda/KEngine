#include "Matrix4x4.h"
#include <cmath>
#include "Vector3.h"

Matrix4x4::Matrix4x4()
{
	// 行列要素の初期化
	m[0][0] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;

	m[1][0] = 0.0f;
	m[1][1] = 1.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;

	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][2] = 1.0f;
	m[2][3] = 0.0f;

	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
	m[3][3] = 1.0f;
}

Matrix4x4::Matrix4x4(const Matrix4x4& mat)
{
	// 値の代入
	*this = mat;
}

Matrix4x4 Matrix4x4::operator+(const Matrix4x4& mat) const noexcept
{
	// 結果格納用
	Matrix4x4 result;

	// 計算を行う
	result.m[0][0] = this->m[0][0] + mat.m[0][0];
	result.m[1][0] = this->m[1][0] + mat.m[1][0];
	result.m[2][0] = this->m[2][0] + mat.m[2][0];
	result.m[3][0] = this->m[3][0] + mat.m[3][0];

	result.m[0][1] = this->m[0][1] + mat.m[0][1];
	result.m[1][1] = this->m[1][1] + mat.m[1][1];
	result.m[2][1] = this->m[2][1] + mat.m[2][1];
	result.m[3][1] = this->m[3][1] + mat.m[3][1];

	result.m[0][2] = this->m[0][2] + mat.m[0][2];
	result.m[1][2] = this->m[1][2] + mat.m[1][2];
	result.m[2][2] = this->m[2][2] + mat.m[2][2];
	result.m[3][2] = this->m[3][2] + mat.m[3][2];

	result.m[0][3] = this->m[0][3] + mat.m[0][3];
	result.m[1][3] = this->m[1][3] + mat.m[1][3];
	result.m[2][3] = this->m[2][3] + mat.m[2][3];
	result.m[3][3] = this->m[3][3] + mat.m[3][3];

	// 計算後の値を返還する
	return result;
}

Matrix4x4 Matrix4x4::operator-(const Matrix4x4& mat) const noexcept
{
	// 結果格納用
	Matrix4x4 result;

	// 計算を行う
	result.m[0][0] = this->m[0][0] - mat.m[0][0];
	result.m[1][0] = this->m[1][0] - mat.m[1][0];
	result.m[2][0] = this->m[2][0] - mat.m[2][0];
	result.m[3][0] = this->m[3][0] - mat.m[3][0];

	result.m[0][1] = this->m[0][1] - mat.m[0][1];
	result.m[1][1] = this->m[1][1] - mat.m[1][1];
	result.m[2][1] = this->m[2][1] - mat.m[2][1];
	result.m[3][1] = this->m[3][1] - mat.m[3][1];

	result.m[0][2] = this->m[0][2] - mat.m[0][2];
	result.m[1][2] = this->m[1][2] - mat.m[1][2];
	result.m[2][2] = this->m[2][2] - mat.m[2][2];
	result.m[3][2] = this->m[3][2] - mat.m[3][2];

	result.m[0][3] = this->m[0][3] - mat.m[0][3];
	result.m[1][3] = this->m[1][3] - mat.m[1][3];
	result.m[2][3] = this->m[2][3] - mat.m[2][3];
	result.m[3][3] = this->m[3][3] - mat.m[3][3];

	// 計算後の値を返還する
	return result;
}

Matrix4x4 Matrix4x4::operator*(const Matrix4x4& mat) const noexcept
{
	// 結果格納用
	Matrix4x4 result;

	// 計算を行う
	result.m[0][0] = (this->m[0][0] * mat.m[0][0]) + (this->m[0][1] * mat.m[1][0]) + (this->m[0][2] * mat.m[2][0]) + (this->m[0][3] * mat.m[3][0]);
	result.m[1][0] = (this->m[1][0] * mat.m[0][0]) + (this->m[1][1] * mat.m[1][0]) + (this->m[1][2] * mat.m[2][0]) + (this->m[1][3] * mat.m[3][0]);
	result.m[2][0] = (this->m[2][0] * mat.m[0][0]) + (this->m[2][1] * mat.m[1][0]) + (this->m[2][2] * mat.m[2][0]) + (this->m[2][3] * mat.m[3][0]);
	result.m[3][0] = (this->m[3][0] * mat.m[0][0]) + (this->m[3][1] * mat.m[1][0]) + (this->m[3][2] * mat.m[2][0]) + (this->m[3][3] * mat.m[3][0]);

	result.m[0][1] = (this->m[0][0] * mat.m[0][1]) + (this->m[0][1] * mat.m[1][1]) + (this->m[0][2] * mat.m[2][1]) + (this->m[0][3] * mat.m[3][1]);
	result.m[1][1] = (this->m[1][0] * mat.m[0][1]) + (this->m[1][1] * mat.m[1][1]) + (this->m[1][2] * mat.m[2][1]) + (this->m[1][3] * mat.m[3][1]);
	result.m[2][1] = (this->m[2][0] * mat.m[0][1]) + (this->m[2][1] * mat.m[1][1]) + (this->m[2][2] * mat.m[2][1]) + (this->m[2][3] * mat.m[3][1]);
	result.m[3][1] = (this->m[3][0] * mat.m[0][1]) + (this->m[3][1] * mat.m[1][1]) + (this->m[3][2] * mat.m[2][1]) + (this->m[3][3] * mat.m[3][1]);

	result.m[0][2] = (this->m[0][0] * mat.m[0][2]) + (this->m[0][1] * mat.m[1][2]) + (this->m[0][2] * mat.m[2][2]) + (this->m[0][3] * mat.m[3][2]);
	result.m[1][2] = (this->m[1][0] * mat.m[0][2]) + (this->m[1][1] * mat.m[1][2]) + (this->m[1][2] * mat.m[2][2]) + (this->m[1][3] * mat.m[3][2]);
	result.m[2][2] = (this->m[2][0] * mat.m[0][2]) + (this->m[2][1] * mat.m[1][2]) + (this->m[2][2] * mat.m[2][2]) + (this->m[2][3] * mat.m[3][2]);
	result.m[3][2] = (this->m[3][0] * mat.m[0][2]) + (this->m[3][1] * mat.m[1][2]) + (this->m[3][2] * mat.m[2][2]) + (this->m[3][3] * mat.m[3][2]);

	result.m[0][3] = (this->m[0][0] * mat.m[0][3]) + (this->m[0][1] * mat.m[1][3]) + (this->m[0][2] * mat.m[2][3]) + (this->m[0][3] * mat.m[3][3]);
	result.m[1][3] = (this->m[1][0] * mat.m[0][3]) + (this->m[1][1] * mat.m[1][3]) + (this->m[1][2] * mat.m[2][3]) + (this->m[1][3] * mat.m[3][3]);
	result.m[2][3] = (this->m[2][0] * mat.m[0][3]) + (this->m[2][1] * mat.m[1][3]) + (this->m[2][2] * mat.m[2][3]) + (this->m[2][3] * mat.m[3][3]);
	result.m[3][3] = (this->m[3][0] * mat.m[0][3]) + (this->m[3][1] * mat.m[1][3]) + (this->m[3][2] * mat.m[2][3]) + (this->m[3][3] * mat.m[3][3]);

	// 計算後の値を返還する
	return result;
}

Matrix4x4& Matrix4x4::operator=(const Matrix4x4& mat) noexcept
{
	// 値の代入
	this->m[0][0] = mat.m[0][0];
	this->m[1][0] = mat.m[1][0];
	this->m[2][0] = mat.m[2][0];
	this->m[3][0] = mat.m[3][0];

	this->m[0][1] = mat.m[0][1];
	this->m[1][1] = mat.m[1][1];
	this->m[2][1] = mat.m[2][1];
	this->m[3][1] = mat.m[3][1];

	this->m[0][2] = mat.m[0][2];
	this->m[1][2] = mat.m[1][2];
	this->m[2][2] = mat.m[2][2];
	this->m[3][2] = mat.m[3][2];

	this->m[0][3] = mat.m[0][3];
	this->m[1][3] = mat.m[1][3];
	this->m[2][3] = mat.m[2][3];
	this->m[3][3] = mat.m[3][3];

	// 代入した値を返還する
	return *this;
}

Matrix4x4& Matrix4x4::operator=(Matrix4x4&& mat) noexcept
{
	// 値の代入
	this->m[0][0] = std::move(mat.m[0][0]);
	this->m[1][0] = std::move(mat.m[1][0]);
	this->m[2][0] = std::move(mat.m[2][0]);
	this->m[3][0] = std::move(mat.m[3][0]);

	this->m[0][1] = std::move(mat.m[0][1]);
	this->m[1][1] = std::move(mat.m[1][1]);
	this->m[2][1] = std::move(mat.m[2][1]);
	this->m[3][1] = std::move(mat.m[3][1]);

	this->m[0][2] = std::move(mat.m[0][2]);
	this->m[1][2] = std::move(mat.m[1][2]);
	this->m[2][2] = std::move(mat.m[2][2]);
	this->m[3][2] = std::move(mat.m[3][2]);

	this->m[0][3] = std::move(mat.m[0][3]);
	this->m[1][3] = std::move(mat.m[1][3]);
	this->m[2][3] = std::move(mat.m[2][3]);
	this->m[3][3] = std::move(mat.m[3][3]);

	// 代入した値を返還する
	return *this;
}

Matrix4x4 Matrix4x4::operator+=(const Matrix4x4& mat) noexcept
{
	// 計算を行う
	this->m[0][0] += mat.m[0][0];
	this->m[1][0] += mat.m[1][0];
	this->m[2][0] += mat.m[2][0];
	this->m[3][0] += mat.m[3][0];

	this->m[0][1] += mat.m[0][1];
	this->m[1][1] += mat.m[1][1];
	this->m[2][1] += mat.m[2][1];
	this->m[3][1] += mat.m[3][1];

	this->m[0][2] += mat.m[0][2];
	this->m[1][2] += mat.m[1][2];
	this->m[2][2] += mat.m[2][2];
	this->m[3][2] += mat.m[3][2];

	this->m[0][3] += mat.m[0][3];
	this->m[1][3] += mat.m[1][3];
	this->m[2][3] += mat.m[2][3];
	this->m[3][3] += mat.m[3][3];

	// 計算後の値を返還する
	return *this;
}

Matrix4x4 Matrix4x4::operator-=(const Matrix4x4& mat) noexcept
{
	// 計算を行う
	this->m[0][0] -= mat.m[0][0];
	this->m[1][0] -= mat.m[1][0];
	this->m[2][0] -= mat.m[2][0];
	this->m[3][0] -= mat.m[3][0];

	this->m[0][1] -= mat.m[0][1];
	this->m[1][1] -= mat.m[1][1];
	this->m[2][1] -= mat.m[2][1];
	this->m[3][1] -= mat.m[3][1];

	this->m[0][2] -= mat.m[0][2];
	this->m[1][2] -= mat.m[1][2];
	this->m[2][2] -= mat.m[2][2];
	this->m[3][2] -= mat.m[3][2];

	this->m[0][3] -= mat.m[0][3];
	this->m[1][3] -= mat.m[1][3];
	this->m[2][3] -= mat.m[2][3];
	this->m[3][3] -= mat.m[3][3];

	// 計算後の値を返還する
	return *this;
}

const Matrix4x4& Matrix4x4::Identity()
{
	// 単位行列の生成
	this->m[0][0] = 1.0f;
	this->m[0][1] = 0.0f;
	this->m[0][2] = 0.0f;
	this->m[0][3] = 0.0f;

	this->m[1][0] = 0.0f;
	this->m[1][1] = 1.0f;
	this->m[1][2] = 0.0f;
	this->m[1][3] = 0.0f;

	this->m[2][0] = 0.0f;
	this->m[2][1] = 0.0f;
	this->m[2][2] = 1.0f;
	this->m[2][3] = 0.0f;

	this->m[3][0] = 0.0f;
	this->m[3][1] = 0.0f;
	this->m[3][2] = 0.0f;
	this->m[3][3] = 1.0f;

	// 生成した単位行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::Scale(const Vector3& scale)
{
	// 単位行列生成
	Identity();

	// 行列の各要素に引数の値を代入する
	this->m[0][0] = scale.x;
	this->m[1][1] = scale.y;
	this->m[2][2] = scale.z;

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::RotateX(const float& x)
{
	// 単位行列生成
	Identity();

	// 行列の各要素に引数の値を代入する
	this->m[1][1] = std::cosf(x);
	this->m[1][2] = std::sinf(x);
	this->m[2][1] = -(std::sinf(x));
	this->m[2][2] = std::cosf(x);

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::RotateY(const float& y)
{
	// 単位行列生成
	Identity();

	// 行列の各要素に引数の値を代入する
	this->m[0][0] = std::cosf(y);
	this->m[0][2] = -(std::sinf(y));
	this->m[2][0] = std::sinf(y);
	this->m[2][2] = std::cosf(y);

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::RotateZ(const float& z)
{
	// 単位行列生成
	Identity();

	// 行列の各要素に引数の値を代入する
	this->m[0][0] = std::cosf(z);
	this->m[0][1] = std::sinf(z);
	this->m[1][0] = -(std::sinf(z));
	this->m[1][1] = std::cosf(z);

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::Rotate(const Vector3& rotate)
{
	// 単位行列生成
	Identity();

	// 計算処理
	Matrix4x4 tempRotateXMatrix = RotateX(rotate.x);
	Matrix4x4 tempRotateYMatrix = RotateY(rotate.y);
	Matrix4x4 tempRotateZMatrix = RotateZ(rotate.z);

	// 計算した行列を合成する
	*this = tempRotateXMatrix * tempRotateYMatrix * tempRotateZMatrix;

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::Translate(const Vector3& translate)
{
	// 単位行列生成
	Identity();

	// 計算処理
	this->m[3][0] = translate.x;
	this->m[3][1] = translate.y;
	this->m[3][2] = translate.z;

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::Affin(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	// 単位行列生成
	Identity();

	// 計算処理
	Matrix4x4 tempScaleMatrix = Scale(scale);
	Matrix4x4 tempRotateMatrix = Rotate(rotate);
	Matrix4x4 tempTranslateMatrix = Translate(translate);

	// 計算した行列を合成する
	*this = tempScaleMatrix * tempRotateMatrix * tempTranslateMatrix;

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::Inverse()
{
	// 行列式格納用
	float d;

	// 行列式を求める
	d = (this->m[0][0] * this->m[1][1] * this->m[2][2] * this->m[3][3]) +
		(this->m[0][0] * this->m[1][2] * this->m[2][3] * this->m[3][1]) +
		(this->m[0][0] * this->m[1][3] * this->m[2][1] * this->m[3][2]) -

		(this->m[0][0] * this->m[1][3] * this->m[2][2] * this->m[3][1]) -
		(this->m[0][0] * this->m[1][2] * this->m[2][1] * this->m[3][3]) -
		(this->m[0][0] * this->m[1][1] * this->m[2][3] * this->m[3][2]) -

		(this->m[0][1] * this->m[1][0] * this->m[2][2] * this->m[3][3]) -
		(this->m[0][2] * this->m[1][0] * this->m[2][3] * this->m[3][1]) -
		(this->m[0][3] * this->m[1][0] * this->m[2][1] * this->m[3][2]) +

		(this->m[0][3] * this->m[1][0] * this->m[2][2] * this->m[3][1]) +
		(this->m[0][2] * this->m[1][0] * this->m[2][1] * this->m[3][3]) +
		(this->m[0][1] * this->m[1][0] * this->m[2][3] * this->m[3][2]) +

		(this->m[0][1] * this->m[1][2] * this->m[2][0] * this->m[3][3]) +
		(this->m[0][2] * this->m[1][3] * this->m[2][0] * this->m[3][1]) +
		(this->m[0][3] * this->m[1][1] * this->m[2][0] * this->m[3][2]) -

		(this->m[0][3] * this->m[1][2] * this->m[2][0] * this->m[3][1]) -
		(this->m[0][2] * this->m[1][1] * this->m[2][0] * this->m[3][3]) -
		(this->m[0][1] * this->m[1][3] * this->m[2][0] * this->m[3][2]) -

		(this->m[0][1] * this->m[1][2] * this->m[2][3] * this->m[3][0]) -
		(this->m[0][2] * this->m[1][3] * this->m[2][1] * this->m[3][0]) -
		(this->m[0][3] * this->m[1][1] * this->m[2][2] * this->m[3][0]) +

		(this->m[0][3] * this->m[1][2] * this->m[2][1] * this->m[3][0]) +
		(this->m[0][2] * this->m[1][1] * this->m[2][3] * this->m[3][0]) +
		(this->m[0][1] * this->m[1][3] * this->m[2][2] * this->m[3][0]);

	// 計算結果の格納用
	Matrix4x4 result;

	// 計算を行う
	if (d != 0) {

		result.m[0][0] = (1.0f / d) *
			(this->m[1][1] * this->m[2][2] * this->m[3][3] + this->m[1][2] * this->m[2][3] * this->m[3][1] + this->m[1][3] * this->m[2][1] * this->m[3][2]
				- this->m[1][3] * this->m[2][2] * this->m[3][1] - this->m[1][2] * this->m[2][1] * this->m[3][3] - this->m[1][1] * this->m[2][3] * this->m[3][2]);
		result.m[0][1] = (1.0f / d) *
			(-this->m[0][1] * this->m[2][2] * this->m[3][3] - this->m[0][2] * this->m[2][3] * this->m[3][1] - this->m[0][3] * this->m[2][1] * this->m[3][2]
				+ this->m[0][3] * this->m[2][2] * this->m[3][1] + this->m[0][2] * this->m[2][1] * this->m[3][3] + this->m[0][1] * this->m[2][3] * this->m[3][2]);
		result.m[0][2] = (1.0f / d) *
			(+this->m[0][1] * this->m[1][2] * this->m[3][3] + this->m[0][2] * this->m[1][3] * this->m[3][1] + this->m[0][3] * this->m[1][1] * this->m[3][2]
				- this->m[0][3] * this->m[1][2] * this->m[3][1] - this->m[0][2] * this->m[1][1] * this->m[3][3] - this->m[0][1] * this->m[1][3] * this->m[3][2]);
		result.m[0][3] = (1.0f / d) *
			(-this->m[0][1] * this->m[1][2] * this->m[2][3] - this->m[0][2] * this->m[1][3] * this->m[2][1] - this->m[0][3] * this->m[1][1] * this->m[2][2]
				+ this->m[0][3] * this->m[1][2] * this->m[2][1] + this->m[0][2] * this->m[1][1] * this->m[2][3] + this->m[0][1] * this->m[1][3] * this->m[2][2]);


		result.m[1][0] = (1.0f / d) *
			(-this->m[1][0] * this->m[2][2] * this->m[3][3] - this->m[1][2] * this->m[2][3] * this->m[3][0] - this->m[1][3] * this->m[2][0] * this->m[3][2]
				+ this->m[1][3] * this->m[2][2] * this->m[3][0] + this->m[1][2] * this->m[2][0] * this->m[3][3] + this->m[1][0] * this->m[2][3] * this->m[3][2]);
		result.m[1][1] = (1.0f / d) *
			(this->m[0][0] * this->m[2][2] * this->m[3][3] + this->m[0][2] * this->m[2][3] * this->m[3][0] + this->m[0][3] * this->m[2][0] * this->m[3][2]
				- this->m[0][3] * this->m[2][2] * this->m[3][0] - this->m[0][2] * this->m[2][0] * this->m[3][3] - this->m[0][0] * this->m[2][3] * this->m[3][2]);
		result.m[1][2] = (1.0f / d) *
			(-this->m[0][0] * this->m[1][2] * this->m[3][3] - this->m[0][2] * this->m[1][3] * this->m[3][0] - this->m[0][3] * this->m[1][0] * this->m[3][2]
				+ this->m[0][3] * this->m[1][2] * this->m[3][0] + this->m[0][2] * this->m[1][0] * this->m[3][3] + this->m[0][0] * this->m[1][3] * this->m[3][2]);
		result.m[1][3] = (1.0f / d) *
			(this->m[0][0] * this->m[1][2] * this->m[2][3] + this->m[0][2] * this->m[1][3] * this->m[2][0] + this->m[0][3] * this->m[1][0] * this->m[2][2]
				- this->m[0][3] * this->m[1][2] * this->m[2][0] - this->m[0][2] * this->m[1][0] * this->m[2][3] - this->m[0][0] * this->m[1][3] * this->m[2][2]);


		result.m[2][0] = (1.0f / d) *
			(this->m[1][0] * this->m[2][1] * this->m[3][3] + this->m[1][1] * this->m[2][3] * this->m[3][0] + this->m[1][3] * this->m[2][0] * this->m[3][1]
				- this->m[1][3] * this->m[2][1] * this->m[3][0] - this->m[1][1] * this->m[2][0] * this->m[3][3] - this->m[1][0] * this->m[2][3] * this->m[3][1]);
		result.m[2][1] = (1.0f / d) *
			(-this->m[0][0] * this->m[2][1] * this->m[3][3] - this->m[0][1] * this->m[2][3] * this->m[3][0] - this->m[0][3] * this->m[2][0] * this->m[3][1]
				+ this->m[0][3] * this->m[2][1] * this->m[3][0] + this->m[0][1] * this->m[2][0] * this->m[3][3] + this->m[0][0] * this->m[2][3] * this->m[3][1]);
		result.m[2][2] = (1.0f / d) *
			(this->m[0][0] * this->m[1][1] * this->m[3][3] + this->m[0][1] * this->m[1][3] * this->m[3][0] + this->m[0][3] * this->m[1][0] * this->m[3][1]
				- this->m[0][3] * this->m[1][1] * this->m[3][0] - this->m[0][1] * this->m[1][0] * this->m[3][3] - this->m[0][0] * this->m[1][3] * this->m[3][1]);
		result.m[2][3] = (1.0f / d) *
			(-this->m[0][0] * this->m[1][1] * this->m[2][3] - this->m[0][1] * this->m[1][3] * this->m[2][0] - this->m[0][3] * this->m[1][0] * this->m[2][1]
				+ this->m[0][3] * this->m[1][1] * this->m[2][0] + this->m[0][1] * this->m[1][0] * this->m[2][3] + this->m[0][0] * this->m[1][3] * this->m[2][1]);


		result.m[3][0] = (1.0f / d) *
			(-this->m[1][0] * this->m[2][1] * this->m[3][2] - this->m[1][1] * this->m[2][2] * this->m[3][0] - this->m[1][2] * this->m[2][0] * this->m[3][1]
				+ this->m[1][2] * this->m[2][1] * this->m[3][0] + this->m[1][1] * this->m[2][0] * this->m[3][2] + this->m[1][0] * this->m[2][2] * this->m[3][1]);
		result.m[3][1] = (1.0f / d) *
			(this->m[0][0] * this->m[2][1] * this->m[3][2] + this->m[0][1] * this->m[2][2] * this->m[3][0] + this->m[0][2] * this->m[2][0] * this->m[3][1]
				- this->m[0][2] * this->m[2][1] * this->m[3][0] - this->m[0][1] * this->m[2][0] * this->m[3][2] - this->m[0][0] * this->m[2][2] * this->m[3][1]);
		result.m[3][2] = (1.0f / d) *
			(-this->m[0][0] * this->m[1][1] * this->m[3][2] - this->m[0][1] * this->m[1][2] * this->m[3][0] - this->m[0][2] * this->m[1][0] * this->m[3][1]
				+ this->m[0][2] * this->m[1][1] * this->m[3][0] + this->m[0][1] * this->m[1][0] * this->m[3][2] + this->m[0][0] * this->m[1][2] * this->m[3][1]);
		result.m[3][3] = (1.0f / d) *
			(this->m[0][0] * this->m[1][1] * this->m[2][2] + this->m[0][1] * this->m[1][2] * this->m[2][0] + this->m[0][2] * this->m[1][0] * this->m[2][1]
				- this->m[0][2] * this->m[1][1] * this->m[2][0] - this->m[0][1] * this->m[1][0] * this->m[2][2] - this->m[0][0] * this->m[1][2] * this->m[2][1]);

	}

	// 現在の行列に代入
	*this = result;

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::OrthGraphic(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip)
{
	// 単位行列生成
	Identity();

	// 計算処理
	this->m[0][0] = 2 / (right - left);
	this->m[1][1] = 2 / (top - bottom);
	this->m[2][2] = 1 / (farClip - nearClip);
	this->m[3][0] = (left + right) / (left - right);
	this->m[3][1] = (top + bottom) / (bottom - top);
	this->m[3][2] = nearClip / (nearClip - farClip);

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::PerspectiveFov(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip)
{
	// 単位行列生成
	Identity();

	// 計算処理
	this->m[0][0] = (1 / aspectRatio) * (1 / tanf(fovY / 2));
	this->m[1][1] = (1 / tanf(fovY / 2));
	this->m[2][2] = farClip / (farClip - nearClip);
	this->m[2][3] = 1.0f;
	this->m[3][2] = -(nearClip * farClip) / (farClip - nearClip);
	this->m[3][3] = 0.0f;

	// 生成した行列の返還
	return *this;
}

const Matrix4x4& Matrix4x4::Viewport(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	// 単位行列生成
	Identity();

	// 計算処理
	this->m[0][0] = width / 2;
	this->m[1][1] = -height / 2;
	this->m[2][2] = maxDepth - minDepth;
	this->m[3][0] = left + (width / 2);
	this->m[3][1] = top + (height / 2);
	this->m[3][2] = minDepth;

	// 生成した行列の返還
	return *this;
}

Matrix4x4 Matrix4x4::MakeScale(const Vector3& scale)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.Scale(scale);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateX(const float& x)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.RotateX(x);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateY(const float& y)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.RotateY(y);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeRotateZ(const float& z)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.RotateZ(z);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeRotate(const Vector3& rotate)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.Rotate(rotate);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeTranslate(const Vector3& translate)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.Translate(translate);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeAffin(const Vector3& scale, const Vector3& rotate, const Vector3& translate)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.Affin(scale, rotate, translate);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeInverse(Matrix4x4 mat)
{
	// 結果格納用
	Matrix4x4 result = mat;

	// 行列生成
	result.Inverse();

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeOrthGraphic(const float& left, const float& top, const float& right, const float& bottom, const float& nearClip, const float& farClip)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.OrthGraphic(left, top, right, bottom, nearClip, farClip);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakePerspectiveFov(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.PerspectiveFov(fovY, aspectRatio, nearClip, farClip);

	// 生成結果を返す
	return result;
}

Matrix4x4 Matrix4x4::MakeViewport(float left, float top, float width, float height, float minDepth, float maxDepth)
{
	// 結果格納用
	Matrix4x4 result;

	// 行列生成
	result.Viewport(left, top, width, height, minDepth, maxDepth);

	// 生成結果を返す
	return result;
}

const Matrix4x4 Matrix4x4::kIdentity = Matrix4x4();
