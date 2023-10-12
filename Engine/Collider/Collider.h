#pragma once
#include <list>
#include <string>
#include <memory>
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
	BaseShape* GetColliderShape() { return colliderShape_.get(); }
	/// <summary>
	/// コライダー形状のセッター
	/// </summary>
	/// <param name="colliderShape">セットする形状</param>
	void SetColliderShape(BaseShape* colliderShape) { colliderShape_.reset(colliderShape); }

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
	/// 現在フレーム触れているオブジェクトを全て取得する関数
	/// </summary>
	/// <returns>全ての現在フレーム触れているオブジェクト</returns>
	std::list<BaseObject*> GetNowCollisionObjectList() { return nowCollisionObjects_; }
	/// <summary>
	/// 現在フレームで衝突しているオブジェクトを追加する関数
	/// </summary>
	/// <param name="object">衝突しているオブジェクト</param>
	void AddNowCollisionObject(BaseObject* object) { nowCollisionObjects_.push_back(object); }
	/// <summary>
	/// 現在フレームで衝突しているオブジェクトリストをクリアする関数
	/// </summary>
	void ClearNowCollisionObjectList() { nowCollisionObjects_.clear(); }

	/// <summary>
	/// 前フレーム衝突したオブジェクトがあるかどうかのゲッター
	/// </summary>
	/// <returns>前フレーム衝突したオブジェクトがあるかどうか</returns>
	bool GetPrevCollisionObject();

	/// <summary>
	/// 前フレーム衝突したオブジェクトリストの上書き関数
	/// </summary>
	/// <param name="objectList">前フレーム衝突したオブジェクトリスト</param>
	void OverridePrevCollisionObjectList(std::list<BaseObject*> objectList) { prevCollisionObjects_ = objectList; }
	/// <summary>
	/// 前フレーム衝突したオブジェクトリストのゲッター
	/// </summary>
	/// <returns></returns>
	std::list<BaseObject*> GetPrevCollisionObjectList() { return prevCollisionObjects_; }
	/// <summary>
	/// 前フレーム衝突したオブジェクトリストをクリアする関数
	/// </summary>
	void ClearPrevCollisionObjectList() { prevCollisionObjects_.clear(); }

	/// <summary>
	/// 引数で指定した名前の前フレーム衝突したオブジェクトを削除する関数
	/// </summary>
	/// <param name="name">消したいオブジェクト</param>
	void DeletePrevCollisionObject(const std::string& name);
	/// <summary>
	/// 前フレーム衝突したオブジェクトにその名前のオブジェクトがあるかどうか調べる関数
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	bool GetPrevCollisionObject(std::string name);

private: // メンバ変数

	// 当たり判定形状
	std::unique_ptr<BaseShape> colliderShape_;

	// このコライダーを持つゲームオブジェクト
	BaseObject* gameObject_;

	// 現在フレームで衝突したオブジェクト達
	std::list<BaseObject*> nowCollisionObjects_;
	// 前フレーム衝突したゲームオブジェクト達
	std::list<BaseObject*> prevCollisionObjects_;

};

