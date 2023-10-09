#pragma once
#include <list>
#include "../Math/Math.h"
#include "ShapeList.h"

// クラスの前方宣言
class BaseObject;

/// <summary>
/// 衝突判定用オブジェクト
/// </summary>
class Collider
{
public: // メンバ変数

	/// <summary>
	/// 更新関数(球)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="radius">半径</param>
	void Update(Vector3 center, float radius) {
		colliderShape_->Update(center, radius);
	};

	/// <summary>
	/// 更新関数(AABB)
	/// </summary>
	/// <param name="center">中心座標</param>
	/// <param name="size">AABBサイズ</param>
	virtual void Update(Vector3 center, Vector3 size) {
		colliderShape_->Update(center, size);
	};

public: // アクセッサ等

	/// <summary>
	/// コライダー形状のゲッター
	/// </summary>
	/// <returns>コライダー形状</returns>
	BaseShape* GetColliderShape() { return colliderShape_; }
	/// <summary>
	/// コライダー形状のセッター
	/// </summary>
	/// <param name="colliderShape">セットする形状</param>
	void SetColliderShape(BaseShape* colliderShape) { colliderShape_ = colliderShape; }

	/// <summary>
	/// このコライダーを持つゲームオブジェクトゲッター
	/// </summary>
	/// <returns>ゲームオブジェクト</returns>
	BaseObject* GetGameObject() { return gameObject_; }
	/// <summary>
	/// このコライダーを持つゲームオブジェクトのセッター
	/// </summary>
	/// <param name="gameObject">ゲームオブジェクト</param>
	void SetGameObject(BaseObject* gameObject) { gameObject_ = gameObject; }

	/// <summary>
	/// 前フレーム衝突したオブジェクトがあるかどうかのゲッター
	/// </summary>
	/// <returns>前フレーム衝突したオブジェクトがあるかどうか</returns>
	bool GetPrevCollisionObject() {
		for (Collider* object : prevCollisionObjects_) // 要素が1つでもあった場合Trueを返す
			return true;
		// 要素が1つもなかった場合False
		return false;
	}
	/// <summary>
	/// 前フレーム衝突したオブジェクトリストのゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Collider*> GetPrevCollisionObjectList() { return prevCollisionObjects_; }
	/// <summary>
	/// 前フレーム衝突したオブジェクトのリストに追加する関数
	/// </summary>
	/// <param name="gameObject">前フレーム衝突したオブジェクト</param>
	void AddPrevCollisionObject(Collider* gameObject) { prevCollisionObjects_.push_back(gameObject); };
	/// <summary>
	/// 前フレーム衝突したオブジェクトリストをクリアする関数
	/// </summary>
	void ClearPrevCollisionObjectList() { prevCollisionObjects_.clear(); }

	/// <summary>
	/// 衝突属性(自分)ゲッター
	/// </summary>
	/// <returns>衝突属性(自分)</returns>
	uint32_t GetCollisionAttribute() { return collisionAttribute_; }
	/// <summary>
	/// 衝突属性(自分)セッター
	/// </summary>
	/// <param name="collisionAttribute">設定する衝突属性</param>
	void SetCollisionAttribute(uint32_t collisionAttribute) { collisionAttribute_ = collisionAttribute; }

	/// <summary>
	/// 衝突マスク(相手)ゲッター
	/// </summary>
	/// <returns>衝突マスク(相手)</returns>
	uint32_t GetCollisionMask() { return collisionMask_; }
	/// <summary>
	/// 衝突マスク(相手)セッター
	/// </summary>
	/// <param name="collisionMask">設定する衝突マスク</param>
	void SetCollisionMask(uint32_t collisionMask) { collisionMask_ = collisionMask; }

private: // メンバ変数

	// 当たり判定形状
	BaseShape* colliderShape_;

	// このコライダーを持つゲームオブジェクト
	BaseObject* gameObject_;

	// このゲームオブジェクトが前フレーム衝突したゲームオブジェクト達
	std::list<Collider*> prevCollisionObjects_;

	// 衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xffffffff;

};

