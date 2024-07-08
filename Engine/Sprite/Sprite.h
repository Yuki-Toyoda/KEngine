#pragma once
#include "../Model/SpriteModel.h"

/// <summary>
/// 2Dスプライト
/// </summary>
class Sprite
{
public: // メンバ関数

	// デストラクタ
	~Sprite();

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">スプライト名</param>
	/// <param name="modelData">スプライト用モデルデータ</param>
	void Init(const std::string& name, ModelData* modelData);
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">スプライト名</param>
	/// <param name="modelData">スプライトに使用するモデルデータ</param>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="texture">テクスチャ</param>
	void Init(const std::string& name, ModelData* modelData, const Vector2 position, const Vector2& size, Texture texture);

	/// <summary>
	/// (ユーザー呼び出し禁止) 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	/// スプライトの破壊関数
	/// </summary>
	void Destroy() { isDestroy_ = true; }
	/// <summary>
	/// 破壊トリガー状態ゲッター
	/// </summary>
	/// <returns></returns>
	bool GetIsDestroy() { return isDestroy_; }

public: // パブリックなメンバ変数

	// スプライト名
	std::string name_ = "";

	// 大きさ
	Vector2 scale_;
	// 回転
	float rotate_ = 0.0f;
	// 座標
	Vector2 translate_ = { 0.0f, 0.0f };

	// アンカーポイント
	Vector2 anchorPoint_ = { 0.0f, 0.0f };

	// 色
	Vector4 color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// テクスチャ始点
	Vector2 texBase_ = { 0.0f, 0.0f };
	// テクスチャ幅と高さ
	Vector2 texSize_ = { 100.0f, 100.0f };

private: // メンバ関数

	// デストロイトリガー
	bool isDestroy_ = false;

	// スプライト用モデル
	SpriteModel* model_;

	// ワールドトランスフォーム
	WorldTransform transform_;

};

