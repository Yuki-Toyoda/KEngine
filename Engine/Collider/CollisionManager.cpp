#include "CollisionManager.h"
#include <algorithm>
#include "Engine/GameObject/IObject.h"

CollisionManager* CollisionManager::GetInstance()
{
	static CollisionManager collisionManager;
	return &collisionManager;
}

void CollisionManager::ListClear()
{
	// 全てのコライダーに関して
	for (Collider* collider : colliders_) {
		// 前フレーム衝突したオブジェクトリストをクリア
		collider->ClearPrevCollisionObjectList();
		// 前フレーム衝突したオブジェクトリストを上書き
		collider->OverridePrevCollisionObjectList(collider->GetNowCollisionObjectList());
		// 現在フレーム触れているオブジェクトリストをクリア
		collider->ClearNowCollisionObjectList();
	}

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

		// イテレータBはイテレータAの次の要素から回すことで重複判定を回避する
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); itrB++) {
			// イテレータBからコライダーBを取得
			Collider* colliderB = *itrB;

			// 前フレーム衝突していたオブジェクトがあった場合
			if (colliderA->GetPrevCollisionObject())
				CheckCollisionExit(colliderA); // 非衝突判定検証

			// 前フレーム衝突していたオブジェクトがあった場合
			if (colliderB->GetPrevCollisionObject())
				CheckCollisionExit(colliderB); // 非衝突判定検証

			// 衝突した瞬間を検証
			CheckCollisionEnter(colliderA, colliderB);

			// ペアの当たり判定を検証する
			CheckCollisionPair(colliderA, colliderB, false);

		}
	}
}

bool CollisionManager::CheckCollisionPair(Collider* colliderA, Collider* colliderB, bool isCheckExit, bool isCheckEnter)
{
	// コライダーが所持しているゲームオブジェクトが同一の場合当たり判定を取らない
	if (colliderA->GetGameObject() == colliderB->GetGameObject() ||
		colliderA->GetGameObject()->GetIsDestroy() || colliderB->GetGameObject()->GetIsDestroy() ||
		!colliderA->GetIsActive() || !colliderB->GetIsActive()) {
		return false;
	}

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
		case BaseShape::OBB: // コライダーBがOBBの場合
			// 球とOBBの当たり判定を検証する
			result = IsCollisionSphereWithOBB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
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
	case BaseShape::OBB: // コライダーAがOBBの場合
		switch (colliderB->GetColliderShape()->GetColliderShape())
		{
		case BaseShape::Sphere: // コライダーBが球の場合
			// 球同士の当たり判定を検証する
			result = IsCollisionSphereWithOBB(colliderB->GetColliderShape(), colliderA->GetColliderShape());
			break;
		case BaseShape::AABB: // コライダーBがAABBの場合

			break;
		case BaseShape::OBB: // コライダーBがOBBの場合
			// 球とAABBの当たり判定を検証する
			result = isCollisionOBB(colliderA->GetColliderShape(), colliderB->GetColliderShape());
			break;
		}
		break;
	}

	// 衝突時の関数を呼び出す
	if (result && !isCheckExit) {
		colliderA->GetGameObject()->OnCollision(colliderB, colliderA); // A
		colliderA->AddNowCollisionObject(colliderB); // 前フレーム衝突したオブジェクトリストに衝突しているオブジェクトを追加
		colliderB->GetGameObject()->OnCollision(colliderA, colliderB); // B
		colliderB->AddNowCollisionObject(colliderA); // 前フレーム衝突したオブジェクトリストに衝突しているオブジェクトを追加
	}

	if (result && isCheckEnter) {
		//colliderA->AddNowCollisionObject(colliderB); // 前フレーム衝突したオブジェクトリストに衝突しているオブジェクトを追加
		//colliderB->AddNowCollisionObject(colliderA); // 前フレーム衝突したオブジェクトリストに衝突しているオブジェクトを追加
	}

	return result;

}

void CollisionManager::CheckCollisionEnter(Collider* colliderA, Collider* colliderB)
{
	// 前フレームにそのオブジェクトが衝突していなければ
	if (CheckCollisionPair(colliderA, colliderB, true, true)) {
		if (!colliderA->GetPrevCollisionObject(colliderB->GetGameObject()->GetObjectName()) &&
			!colliderB->GetPrevCollisionObject(colliderA->GetGameObject()->GetObjectName())) {
			// 衝突した関数を呼び出す
			colliderA->GetGameObject()->OnCollisionEnter(colliderB, colliderA);
			colliderB->GetGameObject()->OnCollisionEnter(colliderA, colliderB);
		}
	}
}

bool CollisionManager::CheckCollisionExit(Collider* collider)
{
	// リスト内の全てのコライダーのペアを検証する
	std::list<Collider*> objectList = collider->GetPrevCollisionObjectList();
	std::list<Collider*>::iterator itr = objectList.begin();
	for (; itr != objectList.end(); itr++) {
		// イテレータAからコライダーAを取得する
		Collider* colliderA = *itr;
		if (collider == nullptr || collider->GetGameObject()->GetIsDestroy()) // 
			continue;

		// そのオブジェクトが衝突していなかった場合
		if (!CheckCollisionPair(collider, colliderA, true)) {
			// 非衝突時関数を呼び出す
			collider->GetGameObject()->OnCollisionExit(colliderA, collider);
			collider->DeletePrevCollisionObject(colliderA->GetGameObject()->GetObjectName());

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

bool CollisionManager::isCollisionOBB(BaseShape* shapeA, BaseShape* shapeB)
{
	// 形状Aの各種情報を取得
	std::vector<Vector3> otientatuonsA = shapeA->GetOtientatuons(); // 回転軸の取得
	Vector3 centerA = shapeA->GetCenter();							// 中心座標の取得
	Vector3 sizeA = shapeA->GetSize();								// 大きさ

	// 形状Bの各種情報を取得
	std::vector<Vector3> otientatuonsB = shapeB->GetOtientatuons(); // 回転軸の取得
	Vector3 centerB = shapeB->GetCenter();							// 中心座標の取得
	Vector3 sizeB = shapeB->GetSize();								// 大きさ

	// 軸
	Vector3 axis[15];

	// 面法線
	axis[0] = otientatuonsA[0];
	axis[1] = otientatuonsA[1];
	axis[2] = otientatuonsA[2];
	axis[3] = otientatuonsB[0];
	axis[4] = otientatuonsB[1];
	axis[5] = otientatuonsB[2];

	// クロス積
	axis[6] = Vector3::Cross(otientatuonsA[0], otientatuonsB[0]);
	axis[7] = Vector3::Cross(otientatuonsA[0], otientatuonsB[1]);
	axis[8] = Vector3::Cross(otientatuonsA[0], otientatuonsB[2]);
	axis[9] = Vector3::Cross(otientatuonsA[1], otientatuonsB[0]);
	axis[10] = Vector3::Cross(otientatuonsA[1], otientatuonsB[1]);
	axis[11] = Vector3::Cross(otientatuonsA[1], otientatuonsB[2]);
	axis[12] = Vector3::Cross(otientatuonsA[2], otientatuonsB[0]);
	axis[13] = Vector3::Cross(otientatuonsA[2], otientatuonsB[1]);
	axis[14] = Vector3::Cross(otientatuonsA[2], otientatuonsB[2]);

	// 頂点

	// 回転させる
	Vector3 obbVertex[8];

	obbVertex[0] = { +sizeA.x, +sizeA.y, +sizeA.z };

	obbVertex[1] = { +sizeA.x, +sizeA.y, -sizeA.z };

	obbVertex[2] = { +sizeA.x, -sizeA.y, +sizeA.z };

	obbVertex[3] = { +sizeA.x, -sizeA.y, -sizeA.z };

	obbVertex[4] = { -sizeA.x, +sizeA.y, +sizeA.z };

	obbVertex[5] = { -sizeA.x, +sizeA.y, -sizeA.z };

	obbVertex[6] = { -sizeA.x, -sizeA.y, +sizeA.z };

	obbVertex[7] = { -sizeA.x, -sizeA.y, -sizeA.z };

	Matrix4x4 obbRotateMatrix = Matrix4x4();
	obbRotateMatrix.m[0][0] = otientatuonsA[0].x;
	obbRotateMatrix.m[0][1] = otientatuonsA[0].y;
	obbRotateMatrix.m[0][2] = otientatuonsA[0].z;
	obbRotateMatrix.m[0][3] = 0.0f;
	obbRotateMatrix.m[1][0] = otientatuonsA[1].x;
	obbRotateMatrix.m[1][1] = otientatuonsA[1].y;
	obbRotateMatrix.m[1][2] = otientatuonsA[1].z;
	obbRotateMatrix.m[1][3] = 0.0f;
	obbRotateMatrix.m[2][0] = otientatuonsA[2].x;
	obbRotateMatrix.m[2][1] = otientatuonsA[2].y;
	obbRotateMatrix.m[2][2] = otientatuonsA[2].z;
	obbRotateMatrix.m[2][3] = 0.0f;
	obbRotateMatrix.m[3][0] = 0.0f;
	obbRotateMatrix.m[3][1] = 0.0f;
	obbRotateMatrix.m[3][2] = 0.0f;
	obbRotateMatrix.m[3][3] = 1.0f;

	for (int i = 0; i < 8; i++) {

		obbVertex[i] = obbVertex[i] * obbRotateMatrix;
		obbVertex[i] = obbVertex[i] + centerA;
	}

	Vector3 obb2Vertex[8] = {
		{ +sizeB.x, +sizeB.y, +sizeB.z },
		{ +sizeB.x, +sizeB.y, -sizeB.z },
		{ +sizeB.x, -sizeB.y, +sizeB.z },
		{ +sizeB.x, -sizeB.y, -sizeB.z },
		{ -sizeB.x, +sizeB.y, +sizeB.z },
		{ -sizeB.x, +sizeB.y, -sizeB.z },
		{ -sizeB.x, -sizeB.y, +sizeB.z },
		{ -sizeB.x, -sizeB.y, -sizeB.z }
	};

	Matrix4x4 obb2RotateMatrix = Matrix4x4();
	obb2RotateMatrix.m[0][0] = otientatuonsB[0].x;
	obb2RotateMatrix.m[0][1] = otientatuonsB[0].y;
	obb2RotateMatrix.m[0][2] = otientatuonsB[0].z;
	obb2RotateMatrix.m[0][3] = 0.0f;
	obb2RotateMatrix.m[1][0] = otientatuonsB[1].x;
	obb2RotateMatrix.m[1][1] = otientatuonsB[1].y;
	obb2RotateMatrix.m[1][2] = otientatuonsB[1].z;
	obb2RotateMatrix.m[1][3] = 0.0f;
	obb2RotateMatrix.m[2][0] = otientatuonsB[2].x;
	obb2RotateMatrix.m[2][1] = otientatuonsB[2].y;
	obb2RotateMatrix.m[2][2] = otientatuonsB[2].z;
	obb2RotateMatrix.m[2][3] = 0.0f;
	obb2RotateMatrix.m[3][0] = 0.0f;
	obb2RotateMatrix.m[3][1] = 0.0f;
	obb2RotateMatrix.m[3][2] = 0.0f;
	obb2RotateMatrix.m[3][3] = 1.0f;

	for (int i = 0; i < 8; i++) {

		obb2Vertex[i] = obb2Vertex[i] * obb2RotateMatrix;
		obb2Vertex[i] = obb2Vertex[i] + centerB;
	}

	// 1.頂点を軸に対して射影
	for (int a = 0; a < 15; a++) {
		float min1 = 0.0f;
		float max1 = 0.0f;
		float min2 = 0.0f;
		float max2 = 0.0f;

		for (int v = 0; v < 8; v++) {
			// 一時保存
			float tmp = 0.0f;
			// obb
			tmp = Vector3::Dot(axis[a].Normalize(), obbVertex[v]);
			// 2.射影した点の最大値と最小値を求める
			if (v == 0 || min1 > tmp) {
				min1 = tmp;
			}
			if (v == 0 || max1 < tmp) {
				max1 = tmp;
			}

			// obb2
			tmp = Vector3::Dot(axis[a].Normalize(), obb2Vertex[v]);
			// 2.射影した点の最大値と最小値を求める
			if (v == 0 || min2 > tmp) {
				min2 = tmp;
			}
			if (v == 0 || max2 < tmp) {
				max2 = tmp;
			}
		}

		// 3.差分の形状を分離軸に射影した長さ
		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float LonSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < LonSpan) {
			// 分離しているので分離軸
			return false;
		}
	}

	return true;

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
	float distance = Vector3::Length(closestPoint - sphereCenter);
	// 距離が半径よりも小さければ衝突
	if (distance <= sphereRadius) {
		return true;
	}

	return false;
}

bool CollisionManager::IsCollisionSphereWithOBB(BaseShape* shapeA, BaseShape* shapeB)
{
	// 球の中心座標取得
	Vector3 sphereCenter = shapeA->GetCenter();
	float sphereRadius = shapeA->GetRadius();

	// obbの中心座標取得
	Vector3 obbCenter = shapeB->GetCenter();
	// obbのサイズ取得
	Vector3 obbSize = shapeB->GetSize();
	// obbの座標軸取得
	std::vector<Vector3> otientatuons = shapeB->GetOtientatuons();

	Matrix4x4 t = Matrix4x4::MakeAffin(obbSize, shapeB->GetRotate(), obbCenter);

	// obbのワールド行列を取得
	Matrix4x4 obbWorldMatrix = Matrix4x4();
	obbWorldMatrix.m[0][0] = otientatuons[0].x;
	obbWorldMatrix.m[0][1] = otientatuons[0].y;
	obbWorldMatrix.m[0][2] = otientatuons[0].z;
	obbWorldMatrix.m[0][3] = 0.0f;
	obbWorldMatrix.m[1][0] = otientatuons[1].x;
	obbWorldMatrix.m[1][1] = otientatuons[1].y;
	obbWorldMatrix.m[1][2] = otientatuons[1].z;
	obbWorldMatrix.m[1][3] = 0.0f;
	obbWorldMatrix.m[2][0] = otientatuons[2].x;
	obbWorldMatrix.m[2][1] = otientatuons[2].y;
	obbWorldMatrix.m[2][2] = otientatuons[2].z;
	obbWorldMatrix.m[2][3] = 0.0f;
	obbWorldMatrix.m[3][0] = t.m[3][0];
	obbWorldMatrix.m[3][1] = t.m[3][1];
	obbWorldMatrix.m[3][2] = t.m[3][2];
	obbWorldMatrix.m[3][3] = 1.0f;

	// 逆行列を求める
	Matrix4x4 invObbWorldMatrix = Matrix4x4::MakeInverse(obbWorldMatrix);

	// obbのローカル座標を求める
	Vector3 centerInOBBLocalSpace = sphereCenter * invObbWorldMatrix;

	// aabbのローカル座標
	//Vector3 aabbMin = Vector3(-obbSize.x, -obbSize.y, -obbSize.z);
	Vector3 aabbCenter = Vector3(0.0f, 0.0f, 0.0f);
	AABB aabbLocal;
	aabbLocal.Init(&aabbCenter, &obbSize);

	// 球のローカル座標
	Sphere sphereLocal;
	sphereLocal.Init(&centerInOBBLocalSpace, &sphereRadius);

	// ローカル空間で衝突しているか確認
	return IsCollisionSphereWithAABB(&sphereLocal, &aabbLocal);
}
