#pragma once
#include <string>
#include <vector>
#include "../../Externals/imgui/imgui.h"
#include "WorldTransform.h"
#include "../Primitive/PrimitiveManager.h"

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
		TagPlayer, // プレイヤー
		TagEnemy,  // 敵
	};

public: // メンバ関数

	// 仮想デストラクタ
	virtual ~BaseObject() = default;

	// (ユーザー呼び出し禁止)共通初期化関数
	void PreInitialize();
	// (ユーザー呼び出し禁止)共通更新関数
	void PreUpdate();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">オブジェクト名</param>
	/// <param name="tag">所属タグ</param>
	virtual void Initialize(std::string name, Tag tag) = 0;

	// 更新関数
	virtual void Update() = 0;

	// 描画関数
	virtual void Draw() = 0;

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	virtual void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	/// オブジェクト破壊関数
	/// </summary>
	void Destroy() { isDestroy_ = true; }

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

private: // プライベートなメンバ関数

	/// <summary>
	/// メッシュ追加関数
	/// </summary>
	/// <param name="wt">ワールドトランスフォーム実体</param>
	/// <param name="color">オブジェクト色</param>
	/// <param name="path">モデルまでのディレクトリパス</param>
	/// <param name="fileName">ファイル名</param>
	/// <param name="enableLighting">ライティングを有効にするか</param>
	void AddMesh(WorldTransform* wt, Vector4 color, const std::string& path, const std::string& fileName);

public: // パブリックなメンバ変数

	// 表示状態切り替えトリガー
	bool isActive_;

	// ワールドトランスフォーム
	WorldTransform transform_;

	// メッシュリスト
	std::vector<Mesh*> meshes_;

protected: // 継承メンバ変数

	// 形状マネージャのインスタンス
	PrimitiveManager* primitiveManager_;

	// オブジェクトの所属
	Tag tag_;
	// オブジェクト名
	std::string name_;

	// デストロイトリガー
	bool isDestroy_;

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

};

