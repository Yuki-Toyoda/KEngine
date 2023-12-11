#pragma once
#include "../Primitive/2d/Plane.h"

/// <summary>
/// 2Dスプライト
/// </summary>
class Sprite
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">スプライト名</param>
	void Initialize(const std::string& name);

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="name">スプライト名</param>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="texture">テクスチャ</param>
	void Initialize(const std::string& name, const Vector2 position, const Vector2& size, Texture* texture);

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

	/// <summary>
	/// 表示状態セッター
	/// </summary>
	/// <param name="isActive">表示するか</param>
	void SetIsActive(bool isActive) { plane_->isActive_ = isActive; }
	/// <summary>
	/// 表示状態ゲッター
	/// </summary>
	/// <returns>表示状態</returns>
	bool GetIsActive() { return plane_->isActive_; }

	/// <summary>
	/// ブレンドモードセッター
	/// </summary>
	/// <param name="mode">設定するブレンドモード</param>
	void SetBlendMode(BasePrimitive::kBlendMode mode) { plane_->blendMode_ = mode; }
	/// <summary>
	/// ブレンドモードゲッター
	/// </summary>
	/// <returns>ブレンドモード</returns>
	BasePrimitive::kBlendMode GetBlendMode() { return plane_->blendMode_; }

public: // パブリックなメンバ変数

	// 大きさ
	Vector2 scale_;
	// 回転
	float rotate_;
	// 座標
	Vector2 translate_;

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

	// 形状
	Plane* plane_;

	// ワールドトランスフォーム
	WorldTransform transform_;

	// ImGuiブレンドモード選択用
	int imGuiSelectBlendMode_ = 0;

};

