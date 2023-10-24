#pragma once
#include <string>
#include "../externals/imgui/imgui.h"
#include "../OBJ/OBJ.h"
#include "WorldTransform.h"
#include "../Sprite/SpriteList.h"
#include "../Collider/Collider.h"
#include "../Collider/CollisionManager.h"
#include "../GlobalVariables/GlobalVariables.h"

/// <summary>
/// 全てのゲームオブジェクトの基底クラス
/// </summary>
class BaseObject{
public: // サブクラス

	/// <summary>
	/// オブジェクトのタグ
	/// </summary>
	enum Tag
	{
		tagCamera, // カメラ
		tagPlayer, // プレイヤー
		tagItem, // アイテム
		tagCatapult, // カタパルト
		tagEnemy, // 敵
		tagFloor, // 床
		tagOther, // その他
	};

public: // デストラクタ

	// 暗黙定義されるデストラクタの挙動を行う
	virtual ~BaseObject();

public: // パブリックメンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">オブジェクトの名前</param>
	/// <param name="tag">オブジェクトのタグ</param>
	virtual void Initialize(std::string name, Tag tag);

	// 更新
	virtual void Update();
	
	/// <summary>
	/// objの描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// スプライト描画
	/// </summary>
	virtual void SpriteDraw() {};

public: // アクセッサ等

	/// <summary>
	/// オブジェクトの破壊関数
	/// </summary>
	void Destroy() { isDestroy_ = true; }

	/// <summary>
	/// 表示状態セッター
	/// </summary>
	/// <param name="isActive">設定する表示状態</param>
	void SetIsActive(bool isActive) { isActive_ = isActive; }
	/// <summary>
	/// 表示状態ゲッター
	/// </summary>
	/// <returns>表示状態</returns>
	bool GetIsActive() { return isActive_; }

	/// <summary>
	/// 破壊トリガーのゲッター
	/// </summary>
	/// <returns>破壊トリガーの状態</returns>
	bool GetIsDestroy() { return isDestroy_; }

	/// <summary>
	/// オブジェクト名セッター
	/// </summary>
	/// <returns>オブジェクト名</returns>
	void SetObjectName(std::string objectName) { objectName_ = objectName; }
	/// <summary>
	/// オブジェクト名ゲッター
	/// </summary>
	/// <returns>オブジェクト名</returns>
	std::string GetObjectName() { return objectName_; }

	/// <summary>
	/// オブジェクトタグのセッター
	/// </summary>
	/// <param name="tag">設定するタグ</param>
	void SetObjectTag(Tag tag) { objectTag_ = tag; }
	/// <summary>
	/// オブジェクトタグのゲッター
	/// </summary>
	/// <returns>そのオブジェクトタグ</returns>
	Tag GetObjectTag() { return objectTag_; }

	/// <summary>
	/// デバック有効状態セッター
	/// </summary>
	/// <param name="enable">有効非有効</param>
	void EnableDebug(bool enable) { isDebug_ = enable; }

	/// <summary>
	/// 衝突判定ゲッター
	/// </summary>
	/// <returns>衝突判定</returns>
	Collider* GetCollider() { return collider_.get(); }

	/// <summary>
	/// 色のセッター
	/// </summary>
	/// <param name="color">色</param>
	void SetColor(Vector4 color) { color_ = color; }

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	virtual void AddGlobalVariables() = 0;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	virtual void ApplyGlobalVariables() = 0;

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	virtual void OnCollisionEnter(BaseObject* object) { object; }

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="object">衝突したオブジェクト</param>
	virtual void OnCollision(BaseObject* object) { object; }

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="object">衝突していたオブジェクト</param>
	virtual void OnCollisionExit(BaseObject* object) { object; }

	/// <summary>
	/// obj作成関数
	/// </summary>
	/// <param name="wt">ワールドトランスフォーム</param>
	/// <param name="color">色</param>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="fileName">ファイル名</param>
	/// <param name="enableLighting">ライティング有効非有効</param>
	void AddOBJ(WorldTransform* wt, const Vector4& color, const std::string& directoryPath, const std::string& fileName, bool enableLighting = true);

	/// <summary>
	/// objects_内の全てのobjを描画する関数
	/// </summary>
	void DrawAllOBJ();

public: // パブリックメンバ変数

	// ワールド座標計算
	WorldTransform transform_;
	// OBJのリスト
	std::vector<std::unique_ptr<OBJ>> objects_;

protected: // メンバ変数

	// 調整項目クラス
	GlobalVariables* globalVariables_ = nullptr;
	// 衝突マネージャー			
	CollisionManager* collisionManager_ = nullptr;

	// 衝突判定
	std::unique_ptr<Collider> collider_;

	// 表示、非表示
	bool isActive_;
	// ゲームオブジェクトの破壊トリガー
	bool isDestroy_;
	// オブジェクトの名前
	std::string objectName_;
	// オブジェクトのタグ
	Tag objectTag_;
	// デバックトリガー
	bool isDebug_ = false;

	// 色設定用
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
};