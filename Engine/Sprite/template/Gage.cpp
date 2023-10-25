#include "Gage.h"
#include "../../Resource/Texture/TextureManager.h"

void Gage::Initialize(float* num, float min, float max, Vector4 backEndColor, Vector4 gageColor, Vector2 position, Vector2 size, Vector2 anchorPoint)
{
	// 参照値取得
	intNum_ = num;
	// 最小値と最大値の設定
	min_ = min;
	max_ = max;

	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 中心点設定
	anchorPoint_ = anchorPoint;

	// ゲージ色リセット
	backGroundColor_ = backEndColor;
	gageColor_ = gageColor;

	// ゲージの大きさ設定
	gageSize_ = { 0.0f, size_.y };

	// 表示
	isActive_ = true;

	// 座標設定
	spriteGageBackGround_.reset(Sprite::Create(TextureManager::Load("./Resources", "white1x1.png"), &position_, &size_, &backGroundColor_, { 0.0f, 0.0f }));
	spriteGage_.reset(Sprite::Create(TextureManager::Load("./Resources", "white1x1.png"), &position_, &gageSize_, &gageColor_, { 0.0f, 0.0f }));
}

void Gage::Initialize(float* num, float min, float max, uint32_t backEnd, Vector4 backEndColor, uint32_t gage, Vector4 gageColor, Vector2 position, Vector2 size, Vector2 anchorPoint)
{
	// 参照値取得
	intNum_ = num;
	// 最小値と最大値の設定
	min_ = min;
	max_ = max;

	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 中心点設定
	anchorPoint_ = anchorPoint;

	// ゲージ色リセット
	backGroundColor_ = backEndColor;
	gageColor_ = gageColor;

	// ゲージの大きさ設定
	gageSize_ = { 0.0f, size_.y };

	// 表示
	isActive_ = true;

	// 座標設定
	spriteGageBackGround_.reset(Sprite::Create(backEnd, &position_, &size_, &backGroundColor_, { 0.0f, 0.0f }));
	spriteGage_.reset(Sprite::Create(gage, &position_, &gageSize_, &gageColor_, { 0.0f, 0.0f }));
}

void Gage::Update()
{

	// 大きさ設定
	gageSize_.y = size_.y;

	// 参照値によってサイズを変更
	if (*intNum_ < max_)
		gageSize_.x = Math::Linear(*intNum_, min_, size_.x, max_);
	else {
		gageSize_.x = size_.x;
	}
}

void Gage::Draw()
{
	if (isActive_) {
		spriteGageBackGround_->Draw();
		spriteGage_->Draw();
	}
}
