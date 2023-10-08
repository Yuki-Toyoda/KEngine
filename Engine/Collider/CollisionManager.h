#pragma once
#include <list>
#include "Collider.h"

/// <summary>
/// 衝突マネージャ
/// </summary>
class CollisionManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	CollisionManager() = default;
	~CollisionManager() = default;
	CollisionManager(const CollisionManager&) = delete;
	CollisionManager& operator=(const CollisionManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static CollisionManager* GetInstance();

	/// <summary>
	/// リストクリア関数
	/// </summary>
	void ListClear();

	/// <summary>
	/// リストにコライダーを登録する関数
	/// </summary>
	/// <param name="collider">検証したいコライダー</param>
	void RegisterCollider(Collider* collider);

	/// <summary>
	/// リスト内の全てのコライダーの当たり判定を検証する関数
	/// </summary>
	void CheckAllCollision();

private: // プライベートなメンバ関数

	/// <summary>
	/// 引数で指定されたコライダーの衝突を検証する関数
	/// </summary>
	/// <param name="colliderA">コライダーA</param>
	/// <param name="colliderB">コライダーB</param>
	/// <returns></returns>
	bool CheckCollisionPair(Collider* colliderA, Collider* colliderB);
	
	/// <summary>
	/// 球同士の衝突検証関数
	/// </summary>
	/// <param name="shapeA">形状A</param>
	/// <param name="shapeB">形状B</param>
	/// <returns>衝突しているか</returns>
	bool IsCollisionSphere(BaseShape* shapeA, BaseShape* shapeB);

	/// <summary>
	/// AABB同士の衝突検証関数
	/// </summary>
	/// <param name="shapeA">形状A</param>
	/// <param name="shapeB">形状B</param>
	/// <returns>衝突しているか</returns>
	bool IsCollisionAABB(BaseShape* shapeA, BaseShape* shapeB);

	/// <summary>
	/// 球とAABBの衝突検証関数
	/// </summary>
	/// <param name="shapeA"></param>
	/// <param name="shapeB"></param>
	/// <returns></returns>
	bool IsCollisionSphereWithAABB(BaseShape* shapeA, BaseShape* shapeB);

private: // メンバ変数

	// コライダーリスト
	std::list<Collider*> colliders_;

};

