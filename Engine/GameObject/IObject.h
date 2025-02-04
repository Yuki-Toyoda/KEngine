#pragma once
#include <string>
#include <vector>
#include <memory>
#include <cmath>
#include <numbers>
#include "Externals/imgui/imgui.h"
#include "WorldTransform.h"
#include "Engine/Model/ModelManager.h"
#include "Engine/Sprite/SpriteManager.h"
#include "Engine/Collider/CollisionManager.h"
#include "Engine/Utility/KLib.h"
#include "Engine/Resource/Texture/TextureManager.h"
#include "Engine/Particle/ParticleManager.h"
#include "Engine/Input/Input.h"

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
class IObject
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
	};

public: // メンバ関数

	// コンストラクタ
	IObject() = default;
	// 仮想デストラクタ
	virtual ~IObject() = default;

	// (ユーザー呼び出し禁止)共通初期化関数
	void CommonInit(std::string name, Tag tag);
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

	/// <summary>
	/// 指定した名称のコライダーを取得する関数
	/// </summary>
	/// <param name="name">取得するコライダー名</param>
	/// <returns>コライダー</returns>
	Collider* GetCollider(const std::string& name);

public: // その他関数群

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	virtual void OnCollisionEnter(Collider* target, [[maybe_unused]] Collider* source) { target, source; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	virtual void OnCollision(Collider* target, [[maybe_unused]] Collider* source) { target, source; }

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	virtual void OnCollisionExit(Collider* target, [[maybe_unused]] Collider* source) { target, source; }

protected: // プライベートなメンバ関数

	/// <summary>
	/// 通常モデル追加関数
	/// </summary>
	/// <param name="name">配列上のモデル</param>
	/// <param name="wt">ワールドトランスフォーム</param>
	/// <param name="path">モデルまでのファイルパス</param>
	/// <param name="fileName">モデル名</param>
	/// <param name="enableLighting">ライティング有効トリガー</param>
	void AddNormalModel(const std::string& name, WorldTransform* wt, const std::string& path, const std::string& fileName, bool enableLighting = true);

	/// <summary>
	/// 通常モデル追加関数
	/// </summary>
	/// <param name="name">配列上のモデル</param>
	/// <param name="wt">ワールドトランスフォーム</param>
	/// <param name="path">モデルまでのファイルパス</param>
	/// <param name="fileName">モデル名</param>
	/// <param name="enableLighting">ライティング有効トリガー</param>
	void AddSkiningModel(const std::string& name, WorldTransform* wt, const std::string& path, const std::string& fileName, bool enableLighting = true);

	/// <summary>
	/// スプライト追加関数
	/// </summary>
	/// <param name="name">追加するスプライト名</param>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="texture">テクスチャ</param>
	void AddSprite(const std::string& name, const Vector2& position, const Vector2& size, Texture texture);

public: // パブリックなメンバ変数

	// 表示状態切り替えトリガー
	bool isActive_ = true;

	// ワールドトランスフォーム
	WorldTransform transform_;

	// アニメーション用ローカル行列
	Matrix4x4 localMat_;

	// 通常モデルリスト
	std::map<std::string, NormalModel*> normalModels_;
	// スキニングモデルリスト
	std::map<std::string, SkiningModel*> skiningModels_;
	// スプライトリスト
	std::map<std::string, Sprite*> sprites_;

protected: // 継承メンバ変数
	
	// モデルデータマネージャーのインスタンス
	ModelDataManager* modelDataManager_ = nullptr;
	// モデルマネージャーのインスタンス
	ModelManager*	  modelManager_ = nullptr;
	// 衝突マネージャーのインスタンス
	CollisionManager* collisionManager_ = nullptr;

	// オブジェクトの所属
	Tag tag_ = TagNone;
	// オブジェクト名
	std::string name_;

	// デストロイトリガー
	bool isDestroy_ = false;

	// コライダー
	std::list<std::unique_ptr<Collider>> colliders_;

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
};

/// <summary>
/// BaseObjectを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectObject>
concept IsBaseObject = std::is_base_of<IObject, SelectObject>::value;

