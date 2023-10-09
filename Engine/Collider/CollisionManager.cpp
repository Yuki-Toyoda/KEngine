#include "CollisionManager.h"
#include <algorithm>
#include "../GameObject/BaseObject.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager collisionManager;
	return &collisionManager;
}

void CollisionManager::ListClear()
{
	// コライダーリストをクリアする
	colliders_.clear();
}

void CollisionManager::RegisterCollider(Collider* collider)
{
	// コライダーリストにコライダーを追加する
	colliders_.push_back(collider);
}

void CollisionManager::CheckAllCollision()
{
	// リスト内の全てのコライダーのペアを検証する
	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); itrA++) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itrA;
		// 前フレーム衝突していたオブジェクトがあった場合
		if (colliderA->GetPrevCollisionObject())
			CheckCollisionExit(colliderA);
		// 上の検証が終わった段階でリストをクリア
		colliderA->ClearPrevCollisionObjectList();

		// イテレータBはイテレータAの次の要素から回すことで重複判定を回避する
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); itrB++) {
			// イテレータBからコライダーBを取得
			Collider* colliderB = *itrB;
			// 前フレーム衝突していたオブジェクトがあった場合
			if (colliderB->GetPrevCollisionObject())
				CheckCollisionExit(colliderB);
			// 上の検証が終わった段階でリストをクリア
			colliderB->ClearPrevCollisionObjectList();

			// ペアの当たり判定を検証する
			CheckCollisionPair(colliderA, colliderB, false);

		}
	}
}

bool CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB, bool isCheckExit)
{
	// コライダーの属性とマスクが1つも一致しない場合は判定をとらない
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask()))
		return false;

	// 結果格納用
	bool result = false;
	// 当たり判定を検証する
	switch (colliderA->GetColliderShape()->GetColliderShape())
	{
	case BaseShape::Sphere: // コライダーAが球の場合
		switch (colliderB->GetColliderShape()->GetColliderShape())
		{
		case BaseShape::Sphere: // コライダーBが球の場合
			// 球同士の当たり判定を検証する
			result = IsCollisionSphere(colliderA->GetColliderShape(), colliderB->GetColliderShape());
			break;
		case BaseShape::AABB: // コライダーBがAABBの場合
			// 球とAABBの当たり判定を検証する
			result = IsCollisionSphereWithAABB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
			break;
		}
		break;
	case BaseShape::AABB: // コライダーAがAABBの場合
		switch (colliderB->GetColliderShape()->GetColliderShape())
		{
		case BaseShape::Sphere: // コライダーBが球の場合
			// AABBと球の当たり判定を検証する
			result = IsCollisionSphereWithAABB(colliderB->GetColliderShape(), colliderA->GetColliderShape());
			break;
		case BaseShape::AABB: // コライダーBがAABBの場合
			// AABB同士の当たり判定を検証する
			result = IsCollisionAABB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
			break;
		}
		break;
	}

	// 衝突時の関数を呼び出す
	if (result && !isCheckExit) {
		colliderA->GetGameObject()->OnCollisionEnter(colliderB->GetGameObject()); // A
		colliderA->AddPrevCollisionObject(colliderB); // 前フレーム衝突したオブジェクトリストに衝突しているオブジェクトを追加
		colliderB->GetGameObject()->OnCollisionEnter(colliderA->GetGameObject()); // B
	}

	return result;

}

bool CollisionManager::CheckCollisionExit(Collider* collider)
{
	// リスト内の全てのコライダーのペアを検証する
	std::list<Collider*> objectList = collider->GetPrevCollisionObjectList();
	std::list<Collider*>::iterator itr = objectList.begin();
	for (; itr != objectList.end(); itr++) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itr;
		if (collider == nullptr || collider->GetGameObject() == nullptr) // 
			continue;

		// そのオブジェクトが衝突していなかった場合
		if (!CheckCollisionPair(collider, colliderA, true)) {
			// 非衝突時関数を呼び出す
			collider->GetGameObject()->OnCollisionExit(colliderA->GetGameObject());
			colliderA->GetGameObject()->OnCollisionExit(collider->GetGameObject());

			// 衝突していない
			return true;
		}
	}
	// 衝突している
	return false;
}

bool CollisionManager::IsCollisionSphere(BaseShape* shapeA, BaseShape* shapeB)
{
	// ワールド座標を取得
	Vector3 posA = shapeA->GetCenter();
	Vector3 posB = shapeB->GetCenter();

	// 取得した座標AとBの距離を求める
	float distance = std::sqrtf(
		std::powf(posB.x - posA.x, 2.0f) + std::powf(posB.y - posA.y, 2.0f) +
		std::powf(posB.z - posA.z, 2.0f));

	// 球と球の当たり判定
	if (distance <= shapeA->GetRadius() + shapeB->GetRadius()) {
		return true;
	}

	return false;
}

bool CollisionManager::IsCollisionAABB(BaseShape* shapeA, BaseShape* shapeB)
{
	// AABBの最小点と最大点を取得
	Vector3 posAMin = shapeA->GetMin();
	Vector3 posAMax = shapeA->GetMax();
	Vector3 posBMin = shapeB->GetMin();
	Vector3 posBMax = shapeB->GetMax();

	// 立方体同士の当たり判定をとる
	if ((posAMin.x <= posBMax.x && posAMax.x >= posBMin.x) && // x軸
		(posAMin.y <= posBMax.y && posAMax.y >= posBMin.y) && // y軸
		(posAMin.z <= posBMax.z && posAMax.z >= posBMin.z)) { // z軸

		// 衝突している
		return true;

	}

	// 衝突していない
	return false;

}

bool CollisionManager::IsCollisionSphereWithAABB(BaseShape* shapeA, BaseShape* shapeB)
{
	Vector3 sphereCenter = shapeA->GetCenter();
	float sphereRadius = shapeA->GetRadius();

	Vector3 AABBMin = shapeB->GetMin();
	Vector3 AABBMax = shapeB->GetMax();

	// 最近接点を求める
	Vector3 closestPoint{
		std::clamp(sphereCenter.x, AABBMin.x, AABBMax.x),
		std::clamp(sphereCenter.y, AABBMin.y, AABBMax.y),
		std::clamp(sphereCenter.z, AABBMin.z, AABBMax.z) };

	// 最近接点と球の中心との距離を求める
	float distance = Math::Length(closestPoint - sphereCenter);
	// 距離が半径よりも小さければ衝突
	if (distance <= sphereRadius) {
		return true;
	}

	return false;
}
