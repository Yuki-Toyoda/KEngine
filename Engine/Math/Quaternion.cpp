#include "Quaternion.h"
#include "Math.h"

Quaternion Quaternion::operator*(const Quaternion q) const
{
	Quaternion result;
	result.vec() = Math::Cross(this->vec(), q.vec()) + (this->vec() * q.w) + (q.vec() * this->w);
	result.w = this->w * q.w - Math::Dot(this->vec(), q.vec());
	return result;
}
