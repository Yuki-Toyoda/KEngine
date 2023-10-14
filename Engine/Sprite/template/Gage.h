#pragma once
#include "../Sprite.h"

/// <summary>
/// 簡易的なゲージテンプレート
/// </summary>
class Gage
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="num">参照する値</param>
	/// <param name="min">参照する値の最小値</param>
	/// <param name="max">参照する値の最大値</param>
	/// <param name="backEndColor">背景カラー</param>
	/// <param name="gageColor">ゲージ色</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="anchorPoint">中心点</param>
	void Initialize(float* num, float min, float max, Vector4 backEndColor, Vector4 gageColor, Vector2 position, Vector2 size, Vector2 anchorPoint);

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="num">参照する値</param>
	/// <param name="min">参照する値の最小値</param>
	/// <param name="max">参照する値の最大値</param>
	/// <param name="backEnd">背景テクスチャ</param>
	/// <param name="backEndColor">背景カラー</param>
	/// <param name="gage">ゲージテクスチャ</param>
	/// <param name="gageColor">ゲージ色</param>
	/// <param name="position">ゲージ座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="anchorPoint">中心点</param>
	void Initialize(float* num, float min, float max, 
		uint32_t backEnd, Vector4 backEndColor, uint32_t gage, Vector4 gageColor,
		Vector2 position, Vector2 size, Vector2 anchorPoint);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // アクセッサ等

	/// <summary>
	/// ゲージ背景テクスチャセッター
	/// </summary>
	/// <param name="textureHandle">ゲージ背景テクスチャ</param>
	void SetGageBackGroundTexture(uint32_t textureHandle) { spriteGageBackGround_->SetTextureHandle(textureHandle); }

	/// <summary>
	/// ゲージテクスチャセッター
	/// </summary>
	/// <param name="textureHandle">ゲージテクスチャ</param>
	void SetGageTexture(uint32_t textureHandle) { spriteGage_->SetTextureHandle(textureHandle); }

	/// <summary>
	/// 中心点ゲッター
	/// </summary>
	/// <returns>中心点</returns>
	Vector2 GetAnchorPoint() { return anchorPoint_; }
	/// <summary>
	/// 中心点セッター
	/// </summary>
	/// <param name="anchorPoint">中心点</param>
	void SetAnchorPoint(Vector2 anchorPoint) { anchorPoint_ = anchorPoint; }

	/// <summary>
	/// ゲージ背景色ゲッター
	/// </summary>
	/// <returns>ゲージ背景色</returns>
	Vector4 GetBackGroundColor() { return backGroundColor_; }
	/// <summary>
	/// ゲージ背景色セッター
	/// </summary>
	/// <param name="color">ゲージ背景色</param>
	void SetBackGroundColor(Vector4 color) { backGroundColor_ = color; }

	/// <summary>
	/// ゲージ色ゲッター
	/// </summary>
	/// <returns>ゲージ色</returns>
	Vector4 GetGageColor() { return gageColor_; }
	/// <summary>
	/// ゲージ色セッター
	/// </summary>
	/// <param name="color">ゲージ色</param>
	void SetGageColor(Vector4 color) { gageColor_ = color; }

	/// <summary>
	/// 最小値ゲッター
	/// </summary>
	/// <returns>最小値</returns>
	float GetMin() { return min_; }
	/// <summary>
	/// 最小値セッター
	/// </summary>
	/// <param name="min">最小値</param>
	void SetMin(float min) { min_ = min; }

	/// <summary>
	/// 最大値ゲッター
	/// </summary>
	/// <returns>最大値</returns>
	float GetMax() { return max_; }
	/// <summary>
	/// 最大値セッター
	/// </summary>
	void SetMax(float max) { max_ = max; }

public: // パブリックなメンバ変数

	// 開始座標
	Vector2 position_;
	// ゲージサイズ
	Vector2 size_;

	// 表示非表示
	bool isActive_;

private: // メンバ変数

	// ゲージ用スプライト
	std::unique_ptr<Sprite> spriteGageBackGround_; // ゲージ背景
	std::unique_ptr<Sprite> spriteGage_; // ゲージ本体

	// ゲージ色
	Vector4 backGroundColor_; // 背景
	Vector4 gageColor_; // 本体

	// 中心点
	Vector2 anchorPoint_;

	// 参照値
	float* intNum_ = nullptr;
	// 参照値最小値
	float min_;
	// 参照値最大値
	float max_;
};

