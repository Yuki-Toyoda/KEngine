#pragma once
#include <list>
#include <string>
#include <memory>

#include "ShapeList.h"

// クラスの前方宣言
class BaseObject;

/// <summary>
/// 衝突判定用オブジェクト
/// </summary>
class Collider
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">名前</param>
	/// <param name="shape">形状</param>
	void Init(const std::string& name, std::unique_ptr<BaseShape> shape);

public: // アクセッサ等

	/// <summary>
	/// コライダー名称ゲッター
	/// </summary>
	/// <returns>コライダー名称</returns>
	std::string GetColliderName() { return name_; }
	/// <summary>
	/// コライダー名称セッター
	/// </summary>
	/// <param name="name">コライダー名称</param>
	void SetColliderName(const std::string& name) { name_ = name; }

	/// <summary>
	/// コライダー形状のゲッター
	/// </summary>
	/// <returns>コライダー形状</returns>
	BaseShape* GetColliderShape() { return colliderShape_.get(); }
	/// <summary>
	/// コライダー形状のセッター
	/// </summary>
	/// <param name="colliderShape">セットする形状</param>
	void SetColliderShape(std::unique_ptr<BaseShape> colliderShape) { colliderShape_ = std::move(colliderShape); }

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
	std::list<Collider*> GetNowCollisionObjectList() { return nowCollisionObjects_; }
	/// <summary>
	/// 現在フレームで衝突しているオブジェクトを追加する関数
	/// </summary>
	/// <param name="object">衝突しているオブジェクト</param>
	void AddNowCollisionObject(Collider* object) { nowCollisionObjects_.push_back(object); }
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
	void OverridePrevCollisionObjectList(std::list<Collider*> objectList);
	/// <summary>
	/// 前フレーム衝突したオブジェクトリストのゲッター
	/// </summary>
	/// <returns></returns>
	std::list<Collider*> GetPrevCollisionObjectList() { return prevCollisionObjects_; }
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

	/// <summary>
	/// 有効状態セッター
	/// </summary>
	/// <param name="isActive">有効状態</param>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	/// <summary>
	/// 有効状態ゲッター
	/// </summary>
	/// <returns>有効状態</returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

private: // メンバ変数

	// コライダー名称
	std::string name_;

	// 当たり判定形状
	std::unique_ptr<BaseShape> colliderShape_;

	// このコライダーを持つゲームオブジェクト
	BaseObject* gameObject_;

	// 現在フレームで衝突したコライダー達
	std::list<Collider*> nowCollisionObjects_;
	// 前フレーム衝突したゲームオブジェクト達
	std::list<Collider*> prevCollisionObjects_;

	// コライダーの有効状態
	bool isActive_ = false;
};

