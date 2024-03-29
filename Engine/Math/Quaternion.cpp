#include "Quaternion.h"
#include "Math.h"

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
