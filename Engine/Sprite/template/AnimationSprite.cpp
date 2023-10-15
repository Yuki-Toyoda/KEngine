#include "AnimationSprite.h"

void AnimationSprite::Initialize(uint32_t animationSheet, Vector2 position, Vector2 size, Vector2 anchorPoint, Vector4 color, Vector2 textureHandleSize, bool isLoop, float drawTime)
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
	// 表示
	isActive_ = true;

	// ループするか
	isLoop_ = isLoop;

	// 1フレームごとの描画サイズ設定
	textureHandleSize_ = textureHandleSize;

	// t初期化
	animT_ = 0.0f;
	// 描画秒数取得
	drawTime_ = drawTime;
	// 描画開始フレームリセット
	beginDrawFrame_ = 0;
	// 描画フレーム初期化
	drawFrame_ = 0;

	// スプライト生成
	sprite_.reset(Sprite::Create(animationSheet_, &position_, &size_, &color_, anchorPoint_));
	// スプライトの描画位置設定
	sprite_->SetTextureRect({ 0.0f, 0.0f }, textureHandleSize_);

	// アニメーションフレーム数取得
	float textureMap = sprite_->GetTextureSize().x;
	animationFrame_ = (int)(textureMap / textureHandleSize_.x);
}

void AnimationSprite::Update()
{
	// 再生中トリガーをfalse
	isPlaying_ = false;
	// 終了トリガーをfalse
	isEnd_ = false;

	if (isPlay_) {
		if (animT_ <= drawTime_ * animationFrame_) {
			// 再生中
			isPlaying_ = true;
			// 描画フレームを計算
			drawFrame_ = Math::Linear(animT_, beginDrawFrame_, animationFrame_, drawTime_ * animationFrame_);
			// スプライトの描画範囲を設定
			sprite_->SetTextureRect({ textureHandleSize_.x * drawFrame_, 0.0f }, textureHandleSize_);
			// tを加算
			animT_ += 1.0f / 60.0f;
		}
		else {

			// 終了トリガーTrue
			isEnd_ = true;

			// ループトリガーがTrueならループさせる
			if (isLoop_)
				animT_ = 0.0f;
		}
	}
}

void AnimationSprite::Draw()
{
	// スプライト描画
	if(isActive_)
		sprite_->Draw();
}

void AnimationSprite::ChangeAnimationSheets(uint32_t animationSheet, Vector2 textureSize, bool isReplay)
{
	// スプライトのテクスチャ変更
	sprite_->SetTextureHandle(animationSheet);

	// １フレームごとの画像サイズ設定
	textureHandleSize_ = textureSize;

	// アニメーションフレーム数取得
	float textureMap = sprite_->GetTextureSize().x;
	animationFrame_ = (int)(textureMap / textureHandleSize_.x);

	// 再度最初から再生する場合は再生
	if (isReplay)
		Replay();
}

void AnimationSprite::SetBeginFrame(int frame)
{
	// 開始フレーム設定
	if (frame <= animationFrame_)
		beginDrawFrame_ = frame;
	else
		beginDrawFrame_ = animationFrame_;
}

void AnimationSprite::Replay()
{
	// tリセット
	animT_ = 0.0f;
	// スプライトの描画範囲を最初のフレームに設定
	sprite_->SetTextureRect({ textureHandleSize_.x * (float)beginDrawFrame_, 0.0f }, textureHandleSize_);
}

void AnimationSprite::ChangeSelectedFrame(int frame)
{
	// 選択フレームが最大フレームを超過していなければ
	if (frame <= animationFrame_) {
		// 描画フレームを取得
		animT_ = Math::Linear(frame, 0.0f, drawTime_ * animationFrame_, animationFrame_);
		// スプライトの描画範囲を最初のフレームに設定
		sprite_->SetTextureRect({ textureHandleSize_.x * (float)beginDrawFrame_, 0.0f }, textureHandleSize_);
	} else {
		// 最後のフレームに
		animT_ = drawTime_ * animationFrame_;
		// スプライトの描画範囲を最初のフレームに設定
		sprite_->SetTextureRect({ textureHandleSize_.x * (float)beginDrawFrame_, 0.0f }, textureHandleSize_);
	}
}
