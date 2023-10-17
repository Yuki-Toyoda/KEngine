#include "AnimationSprite.h"

void AnimationSprite::Initialize(uint32_t animationSheet, Vector2 position, Vector2 size, Vector2 anchorPoint, Vector4 color, int animationFrame, Vector2 textureHandleSize, bool isLoop, float drawTime)
{
	// テクスチャ読み込み
	animationSheet_ = animationSheet;

	// 表示
	isActive_ = true;

	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// アンカーポイント設定
	anchorPoint_ = anchorPoint;
	// 色設定
	color_ = color;

	// アニメーション全体のフレーム取得
	animationFrame_ = animationFrame;

	// 再生
	isPlay_ = true;
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

	// シートの縦、横列取得
	Vector2 textureMap = sprite_->GetTextureSize();
	playFrameColumn_ = textureMap / textureHandleSize_;

	// 最初のフレーム列に指定
	beginPlayFrameColumn_ = { 0.0f, 0.0f };
	nowPlayFrameColumn_ = { 0.0f, 0.0f };

}

void AnimationSprite::Update()
{
	// 再生中トリガーをfalse
	isPlaying_ = false;
	// 終了トリガーをfalse
	isEnd_ = false;

	// 前フレーム記録
	prevDrawFrame_ = drawFrame_;

	if (isPlay_) {
		if (animT_ <= drawTime_ * animationFrame_) {
			// 再生中
			isPlaying_ = true;
			// 描画フレームを計算
			drawFrame_ = Math::Linear(animT_, beginDrawFrame_, animationFrame_, drawTime_ * animationFrame_);

			// フレームが進んでいたら
			if (prevDrawFrame_ != drawFrame_ && !isReset_) {
				// 現在再生している横列が横列サイズを超過していなければ
				if (nowPlayFrameColumn_.x < playFrameColumn_.x - 1.0f) {
					// 再生している横列加算
					nowPlayFrameColumn_.x += 1.0f;
				}
				else {

					// 再生している横列リセット
					nowPlayFrameColumn_.x = 0.0f;

					// 現在再生している縦列が縦列サイズを超過していなければ
					if (nowPlayFrameColumn_.y < playFrameColumn_.y) {
						// 再生している縦列加算
						nowPlayFrameColumn_.y += 1.0f;
					}
					else {
						// 再生している縦列リセット
						nowPlayFrameColumn_.y = 0.0f;
					}
				}
			}

			// スプライトの描画範囲を設定
			sprite_->SetTextureRect((textureHandleSize_ * nowPlayFrameColumn_), textureHandleSize_);
			// tを加算
			animT_ += 1.0f / 60.0f;

			// リセットトリガーfalse
			isReset_ = false;

		}
		else {

			// 終了トリガーTrue
			isEnd_ = true;

			// ループトリガーがTrueならループさせる
			if (isLoop_) {
				Replay();
			}
		}
	}
}

void AnimationSprite::Draw()
{
	// スプライト描画
	if(isActive_)
		sprite_->Draw();
}

void AnimationSprite::ChangeAnimationSheets(uint32_t animationSheet, Vector2 textureSize, int animationFrame, int x, int y)
{
	// スプライトのテクスチャ変更
	sprite_->SetTextureHandle(animationSheet);

	animT_ = 0.0f;

	// １フレームごとの画像サイズ設定
	textureHandleSize_ = textureSize;

	// アニメーションフレーム数取得
	animationFrame_ = animationFrame;
	// 全体のフレーム数取得
	int playFrame = x + y;

	// シートの縦、横列取得
	Vector2 textureMap = sprite_->GetTextureSize();
	playFrameColumn_ = textureMap / textureHandleSize_;

	// 開始フレーム設定
	if (playFrame <= animationFrame_) {
		beginDrawFrame_ = playFrame;
		nowPlayFrameColumn_ = { (float)x, (float)y };
	}
	else {
		// 選択した列がスプライトを超過していた場合最後の場所に固定する
		beginDrawFrame_ = animationFrame_;
		nowPlayFrameColumn_ = playFrameColumn_;
	}

	isReset_ = true;

}

void AnimationSprite::Replay()
{
	// tリセット
	animT_ = 0.0f;
	// スプライトの描画範囲を最初のフレームに設定
	nowPlayFrameColumn_ = beginPlayFrameColumn_;
	// 次のフレームは変更しない
	isReset_ = true;
}

void AnimationSprite::ChangeSelectedFrame(int x, int y)
{

	int playX = x - 1;
	int playY = y - 1;

	// 全体のフレーム数取得
	int playFrame = playX + playY;

	// 選択フレームが最大フレームを超過していなければ
	if (playFrame <= animationFrame_) {
		// 描画フレームを取得
		animT_ = Math::Linear((float)playFrame, 0.0f, drawTime_ * animationFrame_, (float)animationFrame_);
		// スプライトの描画範囲を最初のフレームに設定
		nowPlayFrameColumn_ = { (float)playX, (float)playY };
	}
	else {
		// 最後のフレームに
		animT_ = drawTime_ * animationFrame_;
		// スプライトの描画範囲を最初のフレームに設定
		sprite_->SetTextureRect({ textureHandleSize_.x * (float)beginDrawFrame_, 0.0f }, textureHandleSize_);
		nowPlayFrameColumn_ = playFrameColumn_;
	}

	// 次のフレームは変更しない
	isReset_ = true;

}
