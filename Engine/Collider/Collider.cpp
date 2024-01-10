#include "Collider.h"
#include "../GameObject/BaseObject.h"

void Collider::Init(const std::string& name, std::unique_ptr<BaseShape> shape)
{
	name_ = name;
	colliderShape_ = std::move(shape);
}

bool Collider::GetPrevCollisionObject()
{
	// 前フレーム衝突していたオブジェクトが1つでもあればTrue
	if ((int)prevCollisionObjects_.size() > 0)
		return true;
	// 要素が1つもなかった場合False
	return false;
}

void Collider::OverridePrevCollisionObjectList(std::list<Collider*> objectList)
{
	// 破壊フラグの立ったオブジェクトを削除
	objectList.remove_if([](Collider* c) {
		if (c->GetGameObject()->GetIsDestroy()) {
			return true;
		}
		return false;
		});

	prevCollisionObjects_ = objectList;
}

void Collider::DeletePrevCollisionObject(const std::string& name)
{
	// リスト内の全てのコライダーのペアを検証する
	std::list<Collider*>::iterator itr = prevCollisionObjects_.begin();
	for (; itr != prevCollisionObjects_.end(); itr++) {
		// イテレータAからコライダーAを取得する
		Collider* object = *itr;
		// 同名オブジェクトを見つけた場合それを削除
		if (name == object->GetGameObject()->GetObjectName()) {
			prevCollisionObjects_.erase(itr);
			break;
		}
	}
}

bool Collider::GetPrevCollisionObject(std::string name)
{
	// 要素がある場合
	if (GetPrevCollisionObject()) {
		for (Collider* object : prevCollisionObjects_) {
			// 同名オブジェクトを見つけた場合true
			if (name == object->GetGameObject()->GetObjectName())
				return true;
		}
		// 同名オブジェクトを場合False
		return false;
	}
	// そもそも前フレーム衝突していたオブジェクトが存在しない
	return false;
}

void Collider::DisplayImGui()
{
	if (ImGui::TreeNode(name_.c_str())) {
		colliderShape_->DisplayImGui();
		ImGui::TreePop();
	}
}
