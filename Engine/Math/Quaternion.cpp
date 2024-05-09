#include "Quaternion.h"
#include "Vector3.h"

Quaternion::Quaternion()
{
	// 要素の初期化
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	w = 0.0f;
}

Quaternion::Quaternion(const Vector3& v)
{
	// 引数を元に要素を初期化
	x = v.x;
	y = v.y;
	z = v.z;
	w = 0.0f;
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
