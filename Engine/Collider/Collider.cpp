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

void Collider::DeletePrevCollisionObject(const std::string& name)
{
	// リスト内の全てのコライダーのペアを検証する
	std::list<BaseObject*>::iterator itr = prevCollisionObjects_.begin();
	for (; itr != prevCollisionObjects_.end(); itr++) {
		// イテレータAからコライダーAを取得する
		BaseObject* object = *itr;
		// 同名オブジェクトを見つけた場合それを削除
		if (name == object->GetObjectName()) {
			prevCollisionObjects_.erase(itr);
			break;
		}
	}
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
	return false;
}
