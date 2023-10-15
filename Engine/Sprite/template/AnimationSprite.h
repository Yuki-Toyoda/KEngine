#pragma once
#include "../Sprite.h"

/// <summary>
/// アニメーションするスプライトテンプレート
/// </summary>
class AnimationSprite
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="animationSheet">アニメーションシート</param>
	/// <param name="position">座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="anchorPoint">中心点</param>
	/// <param name="color">色</param>
	/// <param name="textureHandleSize">１フレームごとのテクスチャサイズ</param>
	/// <param name="animationFrame">全体のアニメーションフレーム数</param>
	/// <param name="drawTime">(任意)1フレームの描画秒数(初期値 : 1.0f / 60.0f)</param>
	void Initialize(uint32_t animationSheet, Vector2 position, Vector2 size, Vector2 anchorPoint, Vector4 color, Vector2 textureHandleSize, int animationFrame, float drawTime = 1.0f / 60.0f);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // パブリックなメンバ変数

	// 座標
	Vector2 position_;

	// 大きさ
	Vector2 size_;

	// 色
	Vector4 color_;

private: // メンバ変数

	// アンカーポイント
	Vector2 anchorPoint_;

	// スプライトアニメーションに使用するシート
	uint32_t animationSheet_ = 0u;

	// スプライト
	std::unique_ptr<Sprite> sprite_;

	// １フレームごとの画像サイズ
	Vector2 textureHandleSize_;
	// アニメーション全体のフレーム
	int animationFrame_;

	// アニメーション用t
	float animT_;
	// １フレームを何秒描画するか
	float drawTime_;

	// アニメーションシートの幅と高さ
	Vector2 animmationTextureMap_;

};

