#include "AnimationSprite.h"

void AnimationSprite::Initialize(uint32_t animationSheet, Vector2 position, Vector2 size, Vector2 anchorPoint, Vector4 color, Vector2 textureHandleSize, int animationFrame, float drawTime)
{
	// テクスチャ読み込み
	animationSheet_ = animationSheet;

	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// アンカーポイント設定
	anchorPoint_ = anchorPoint;
	// 色設定
	color_ = color;
	// 1フレームごとの描画サイズ設定
	textureHandleSize_ = textureHandleSize;
	// 全体のアニメーションフレーム取得
	animationFrame_ = animationFrame;
	// 描画秒数取得
	drawTime_ = drawTime;

	// t初期化
	animT_ = 0.0f;

	// スプライト生成
	sprite_.reset(Sprite::Create(animationSheet_, &position_, &size_, &color_, anchorPoint_));
	// スプライトの描画位置設定
	sprite_->SetTextureRect({ 0.0f, 0.0f }, textureHandleSize_);

	// アニメーションシートの幅と高さを取得
	Vector2 textureSize = sprite_->GetTextureSize();
	animmationTextureMap_ = textureSize / textureHandleSize_;
}

void AnimationSprite::Update()
{
	

}

void AnimationSprite::Draw()
{
}
