#include "Quaternion.h"
#include "Math.h"

Quaternion Quaternion::operator*(const Quaternion q) const
{
	Quaternion result;
	result.vec() = Math::Cross(this->vec(), q.vec()) + (this->vec() * q.w) + (q.vec() * this->w);
	result.w = this->w * q.w - Math::Dot(this->vec(), q.vec());
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
