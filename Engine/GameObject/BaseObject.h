#pragma once
#include <string>
#include <vector>
#include <memory>
#include "../../Externals/imgui/imgui.h"
#include "WorldTransform.h"
#include "../Primitive/PrimitiveManager.h"
#include "../Sprite/SpriteManager.h"
#include "../Collider/CollisionManager.h"

/// 最小構成
/*
#include "../../BaseObject.h"

/// <summary>
/// 任意のクラス名
/// </summary>
class 任意のクラス名 : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

	// メンバ変数をここに

};
*/

/// <summary>
/// 全オブジェクトの基底クラス
/// </summary>
class BaseObject
{
public: // サブクラス

	/// <summary>
	/// オブジェクトタグ
	/// </summary>
	enum Tag {
		TagNone,   // 無所属
		TagCamera, // カメラ
		TagPlayer, // プレイヤー
		TagWeapon, // 武器
		TagEnemy,  // 敵
		TagFloor,  // 床
		TagChain,//鎖
		TagObstacle,//障害物
	};

public: // メンバ関数

	// コンストラクタ
	BaseObject() = default;
	// 仮想デストラクタ
	virtual ~BaseObject();

	// (ユーザー呼び出し禁止)共通初期化関数
	void PreInitialize(std::string name, Tag tag);
	// (ユーザー呼び出し禁止)共通更新関数
	void PreUpdate();
	// (ユーザー呼び出し禁止)更新後関数
	void PostUpdate();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	/// <param name="tag">所属タグ</param>
	virtual void Init() = 0;

	// 更新関数
	virtual void Update() = 0;

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	virtual void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	/// オブジェクト破壊関数
	/// </summary>
	void Destroy() { isDestroy_ = true; }

	/// <summary>
	/// 破壊トリガー状態のゲッター
	/// </summary>
	/// <returns>破壊トリガー状態</returns>
	bool GetIsDestroy() { return isDestroy_; }

	/// <summary>
	/// 破壊トリガー状態のゲッター
	/// </summary>
	/// <returns>破壊トリガー状態</returns>
	const bool GetIsDestroy() const { return isDestroy_; }

	/// <summary>
	/// オブジェクト名セッター
	/// </summary>
	/// <param name="name">設定するオブジェクト名</param>
	void SetObjectName(std::string name) { name_ = name; }
	/// <summary>
	/// オブジェクト名ゲッター
	/// </summary>
	/// <returns>オブジェクト名</returns>
	const std::string GetObjectName() { return name_; }

	/// <summary>
	/// オブジェクトタグセッター
	/// </summary>
	/// <param name="tag">設定するタグ</param>
	void SetObjectTag(Tag tag) { tag_ = tag; }
	/// <summary>
	/// オブジェクトタグゲッター
	/// </summary>
	/// <returns>オブジェクトタグ</returns>
	const Tag GetObjectTag() { return tag_; }

	/// <summary>
	/// コライダー追加関数(球)
	/// </summary>
	/// <param name="name">追加するコライダー名称</param>
	/// <param name="center">中心座標</param>
	/// <param name="radius">半径</param>
	/// <param name="enable">登録時にコライダーを有効化するか</param>
	void AddColliderSphere(const std::string& name, Vector3* center, float* radius, bool enable = true);

	/// <summary>
	/// コライダー追加関数(AABB)
	/// </summary>
	/// <param name="name">追加するコライダー名称</param>
	/// <param name="center">中心座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="enable">登録時にコライダーを有効化するか</param>
	void AddColliderAABB(const std::string& name, Vector3* center, Vector3* size, bool enable = true);

	/// <summary>
	/// コライダー追加関数(OBB)
	/// </summary>
	/// <param name="name">追加するコライダー名称</param>
	/// <param name="scale">大きさ</param>
	/// <param name="rotate">回転角</param>
	/// <param name="translate">中心座標</param>
	/// <param name="enable">登録時にコライダーを有効化するか</param>
	void AddColliderOBB(const std::string& name, Vector3* scale, Vector3* rotate, Vector3* translate, bool enable = true);

	/// <summary>
	/// 指定した名称のコライダーを削除する関数
	/// </summary>
	/// <param name="name">削除するコライダー</param>
	void DeleteCollider(const std::string& name);

public: // その他関数群

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したコライダー</param>
	virtual void OnCollisionEnter(Collider* collider) { collider; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="collider">衝突したコライダー<</param>
	virtual void OnCollision(Collider* collider) { collider; }

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="collider">衝突していたコライダー<</param>
	virtual void OnCollisionExit(Collider* collider) { collider; }

protected: // プライベートなメンバ関数

	/// <summary>
	/// メッシュ追加関数
	/// </summary>
	/// <param name="wt">ワールドトランスフォーム実体</param>
	/// <param name="color">オブジェクト色</param>
	/// <param name="path">モデルまでのディレクトリパス</param>
	/// <param name="fileName">ファイル名</param>
	/// <param name="enableLighting">ライティングを有効にするか</param>
	void AddMesh(WorldTransform* wt, Vector4& color, const std::string& path, const std::string& fileName, bool enableLighting = true);

	/// <summary>
	/// スプライト追加関数
	/// </summary>
	/// <param name="name">追加するスプライト名</param>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="texture">テクスチャ</param>
	void AddSprite(const std::string& name, const Vector2& position, const Vector2& size, Texture* texture);

public: // パブリックなメンバ変数

	// 表示状態切り替えトリガー
	bool isActive_ = true;

	// ワールドトランスフォーム
	WorldTransform transform_;

	// メッシュリスト
	std::vector<BasePrimitive*> meshes_;
	// スプライトリスト
	std::vector<Sprite*> sprites_;

protected: // 継承メンバ変数

	// 形状マネージャのインスタンス
	PrimitiveManager* primitiveManager_ = nullptr;
	// 衝突マネージャーのインスタンス
	CollisionManager* collisionManager_ = nullptr;

	// オブジェクトの所属
	Tag tag_;
	// オブジェクト名
	std::string name_;

	// デストロイトリガー
	bool isDestroy_;

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// コライダー
	std::list<std::unique_ptr<Collider>> colliders_;
};

/// <summary>
/// BaseObjectを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectObject>
concept IsBaseObject = std::is_base_of<BaseObject, SelectObject>::value;

