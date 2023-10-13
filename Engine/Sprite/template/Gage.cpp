#include "Gage.h"
#include "../../Resource/Texture/TextureManager.h"

void Gage::Initialize(float* num, float min, float max, Vector4 backEndColor, Vector4 gageColor, Vector2 position, Vector2 size, Vector2 anchorPoint)
{
	// 参照値取得
	num_ = num;
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

	// 表示
	isActive_ = true;

	// 座標設定
	Vector2 anchorPosition = { (1.0f - anchorPoint_.x) * position_.x, (1.0f - anchorPoint_.y) * position_.y };
	spriteGageBackGround_.reset(Sprite::Create(TextureManager::Load("./Resources", "white1x1.png"), anchorPosition, backGroundColor_, { 0.0f, 0.0f }));
	spriteGageBackGround_->size_ = size;
	spriteGage_.reset(Sprite::Create(TextureManager::Load("./Resources", "white1x1.png"), anchorPosition, gageColor_, { 0.0f, 0.0f }));
	spriteGage_->size_ = { 0.0f, size.y };
}

void Gage::Initialize(float* num, float min, float max, uint32_t backEnd, Vector4 backEndColor, uint32_t gage, Vector4 gageColor, Vector2 position, Vector2 size, Vector2 anchorPoint)
{
	// 参照値取得
	num_ = num;
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

	// 表示
	isActive_ = true;

	// 座標設定
	Vector2 anchorPosition = { (1.0f - anchorPoint_.x) * position_.x, (1.0f - anchorPoint_.y) * position_.y };
	spriteGageBackGround_.reset(Sprite::Create(backEnd, anchorPosition, backGroundColor_, { 0.0f, 0.0f }));
	spriteGageBackGround_->size_ = size;
	spriteGage_.reset(Sprite::Create(gage, anchorPosition, gageColor_, { 0.0f, 0.0f }));
	spriteGage_->size_ = { 0.0f, size.y };
}

void Gage::Update()
{
	// 座標設定
	Vector2 anchorPosition = { (1.0f - anchorPoint_.x) * position_.x, (1.0f - anchorPoint_.y) * position_.y };
	spriteGageBackGround_->position_ = anchorPosition;
	spriteGageBackGround_->SetColor(backGroundColor_);
	spriteGage_->position_ = anchorPosition;
	spriteGage_->SetColor(gageColor_);

	// 大きさ設定
	spriteGageBackGround_->size_ = size_;
	spriteGage_->size_.y = size_.y;

	// 参照値によってサイズを変更
	if (*num_ < max_)
		spriteGage_->size_.x = Math::Linear(*num_, min_, size_.x, max_);
	else {
		spriteGage_->size_.x = size_.x;
	}
}

void Gage::Draw()
{
	if (isActive_) {
		spriteGageBackGround_->Draw();
		spriteGage_->Draw();
	}
}
