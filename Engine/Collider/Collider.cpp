#include "Collider.h"
#include "../GameObject/BaseObject.h"

bool Collider::GetPrevCollisionObject()
{
	if ((int)prevCollisionObjects_.size() > 0) {
		return true;
	}
	// 要素が1つもなかった場合False
	return false;
}

bool Collider::GetPrevCollisionObject(std::string name)
{
	// 要素がある場合
	if (GetPrevCollisionObject()) {
		for (BaseObject* object : prevCollisionObjects_) {
			// 同名オブジェクトを見つけた場合true
			if (name == object->GetObjectName())
				return true;
		}
		// 同名オブジェクトを場合False
		return false;
	}
	return true;
}
