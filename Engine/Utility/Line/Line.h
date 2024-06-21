#pragma once
#include <string>
#include "../../GameObject/IObject.h"
#include "../../Primitive/PrimitiveManager.h"
#include "../../Resource/Texture/TextureManager.h"

/// <summary>
/// 線描画用クラス
/// </summary>
class Line
{
public: // コンストラクタ等

	// コンストラクタ
	Line() = default;
	// デストラクタ
	~Line();

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="position">線の開始座標</param>
	/// <param name="thickness">線の太さ</param>
	/// <param name="length">線の長さ</param>
	void Init(const std::string& name, const Vector3& position, const Vector2& thickness, const float& length, Texture texture = TextureManager::GetInstance()->GetDefaultTexture());

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

	/// <summary>
	/// <para>コライダー追加関数</para>
	/// 第二引数のオブジェクトに関しては、OnCollision関係のコールバック関数の対象になります
	/// </summary>
	/// <param name="name">コライダー名</param>
	/// <param name="object">コライダーがコールバックを行う対象のオブジェクト</param>
	void AddCollider(const std::string& name, IObject* object);

	/// <summary>
	/// 親子関係セッター。(使い方はワールドトランスフォームと一緒)
	/// </summary>
	/// <param name="parent">セットするワールド座標</param>
	/// <param name="parentType">親子関係フラグ</param>
	void SetParent(WorldTransform* parent, uint8_t parentType = 0b111);

private: // メンバ変数

	// (いじらないでね)ワールド座標
	WorldTransform transform_;
	// 線の色
	Vector4 color_;
	// メッシュ
	OldMesh* mesh_;

	// 線で使うテクスチャ
	Texture* texture_;

public: // パブリックなメンバ変数

	// 線1つの名前
	std::string name_;
	// 線の始点座標
	Vector3 position_;
	// 線の太さ
	Vector2 thickness_;
	// 始端座標からの長さ
	float length_;
	// 回転
	Vector3 rotate_;

	// 親がいる場合のワールド座標
	Vector3 worldPos_;

	// 表示状態
	bool isActive_ = true;

};

