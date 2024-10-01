#include "Quaternion.h"
#include <cmath>
#include <algorithm>
#include "Vector3.h"
#include "Matrix4x4.h"

Quaternion::Quaternion()
{
	// 単位クォータニオンの生成
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 1.0f;
}

Quaternion::Quaternion(const Vector3& v)
{
	// 乗法単位元
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0.0f;
}

Quaternion::Quaternion(float x, float y, float z, float w) noexcept
{
	// 乗法単位元
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

inline Vector3& Quaternion::vec()
{
	return *reinterpret_cast<Vector3*>(&x);
}

inline const Vector3& Quaternion::vec() const
{
	return *reinterpret_cast<const Vector3*>(&x);
}

Quaternion Quaternion::operator+(const Quaternion q) const
{
	Quaternion result = *this;
	result.vec() = { this->x + q.vec().x, this->y + q.vec().y, this->z + q.vec().z };
	result.w = this->w + q.w;
	return result;
}

Quaternion Quaternion::operator*(const Quaternion q) const
{
	Quaternion result;
	result.vec() = Vector3::Cross(this->vec(), q.vec()) + (this->vec() * q.w) + (q.vec() * this->w);
	result.w = this->w * q.w - Vector3::Dot(this->vec(), q.vec());
	return result;
}

Quaternion Quaternion::operator*(const float f) const
{
	Quaternion result = *this;

	result.vec() = result.vec() * f;
	result.w = result.w * f;

	return result;
}

Quaternion Quaternion::operator/(const Quaternion q) const
{
	Quaternion result;
	result.x = this->x / q.x;
	result.y = this->y / q.y;
	result.z = this->z / q.z;
	result.w = this->w / q.w;
	return result;
}

Quaternion Quaternion::Conjugate(const Quaternion& q)
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

float Quaternion::Length(const Quaternion& q)
{
	// 結果格納用
	float result;

	// 計算処理
	result = sqrtf(powf(q.w, 2) + powf(q.x, 2) + powf(q.y, 2) + powf(q.z, 2));

	// 結果を返す
	return result;
}

Quaternion Quaternion::Normalize(const Quaternion& q)
{
	// 正規化するベクトルの長さを求める
	float length = Length(q);
	// 結果格納用
	Quaternion result;

	// 計算処理
	if (length > 1e-6f) {
		result.x = q.x / length;
		result.y = q.y / length;
		result.z = q.z / length;
		result.w = q.w / length;
	}
	else {
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
		result.w = 1.0f;
	}

	// 結果を返す
	return result;
}

float Quaternion::Dot(const Quaternion& q1, const Quaternion& q2)
{
	// 結果格納用
	float result = 0.0f;

	// 計算
	result = (q1.x * q2.x) + (q1.y * q2.y) + (q1.z * q2.z) + (q1.w * q2.w);

	// 結果を返す
	return result;
}

Quaternion Quaternion::Inverse(const Quaternion& q)
{
	// 結果格納用
	Quaternion result = Quaternion();

	// 正規化するベクトルの長さを求める
	float length = Length(q);
	length = std::powf(length, 2);
	// 共役クォータニオンを求める
	Quaternion conjugate = Conjugate(q);

	// 計算処理
	if (length > 1e-6f) {
		result.x = conjugate.x / length;
		result.y = conjugate.y / length;
		result.z = conjugate.z / length;
		result.w = conjugate.w / length;
	}
	else {
		// 無効なクォータニオンの場合は単位クォータニオンを返還する
		result.x = 0.0f;
		result.y = 0.0f;
		result.z = 0.0f;
		result.w = 1.0f;
	}

	// 結果を返す
	return result;
}


Quaternion Quaternion::MakeRotateAxisAngleQuaternion(const Vector3& v, float angle)
{
	// 結果格納用
	Quaternion result = Quaternion();

	// 計算処理
	result.w = cos((angle / 2.0f));
	result.x = v.x * sin((angle / 2.0f));
	result.y = v.y * sin((angle / 2.0f));
	result.z = v.z * sin((angle / 2.0f));

	// 結果を返す
	return result;
}

Vector3 Quaternion::RoatateVector(const Vector3& v, const Quaternion& q)
{
	// 引数のベクトルを使用してクォータニオンを生成する
	Quaternion vq = Quaternion();
	vq.x = v.x;
	vq.y = v.y;
	vq.z = v.z;
	vq.w = 0.0f;

	// 生成したクォータニオンを用いてクォータニオンを回転させる
	Quaternion result = q * vq * Conjugate(q);
	// 計算結果を返す
	return result.vec();
}

Matrix4x4 Quaternion::QuaternionToMatrix(const Quaternion& q)
{
	// 結果格納用
	Matrix4x4 result = Matrix4x4();

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

Quaternion Quaternion::Slerp(float t, const Quaternion& start, const Quaternion& end)
{
	// 結果格納用
	Quaternion result = Quaternion();

	// 値を取得
	Quaternion s = Normalize(start);
	Quaternion e = Normalize(end);

	// クォータニオンの内積で求める
	float dot = Dot(start, end);
	dot = std::clamp(dot, -1.0f, 1.0f);

	if (dot < 0.0f) {
		// 逆の回転を使う
		e = Inverse(e);
		dot = -dot;
	}

	if (dot > 0.9995f) {
		result = s * (1.0f - t) + (e * t);
		return Normalize(result);
	}

	float theta = std::acos(dot);
	float sinTheta = 1.0f / std::sin(theta);
	// しきい値を設定し、非常に小さい角度の場合には線形補間を使用
	if (std::abs(theta) < 1e-6) {
		result = s * (1.0f - t) + (e * t);
		return Normalize(result);
	}

	// 開始と終端を求める
	float fStart = std::sin((1.0f - t) * theta) / sinTheta;	// 開始
	float fEnd = std::sin(t * theta) / sinTheta;			// 終端

	// 補間を行う
	result = (s * fStart) + (e * fEnd);
	// 結果を正規化返す
	return Normalize(result);
}

Matrix4x4 Quaternion::MakeAffine(const Vector3& scale, const Quaternion& rotate, const Vector3& translate)
{
	// 単位行列生成
	Matrix4x4 result = Matrix4x4();

	// 計算処理
	Matrix4x4 tempScaleMatrix = Matrix4x4::MakeScale(scale);
	Matrix4x4 tempRotateMatrix = QuaternionToMatrix(rotate);
	Matrix4x4 tempTranslateMatrix = Matrix4x4::MakeTranslate(translate);

	// 計算した行列を合成する
	result = tempScaleMatrix * tempRotateMatrix * tempTranslateMatrix;

	// 生成した行列の返還
	return result;
}
