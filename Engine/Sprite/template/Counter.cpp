#include "Counter.h"
#include "../../Resource/Texture/TextureManager.h"

void Counter::Initialize(int* num, Vector2 position, Vector2 size, float lineSpace)
{
	// 参照値取得
	intNum_ = num;
	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 行間設定
	lineSpace_ = lineSpace;
	// 色設定
	color_ = {1.0f, 1.0f, 1.0f, 1.0f};

	// 番号のテクスチャサイズ指定
	numberTextureSize_ = { 512.0f, 512.0f };

	// 表示
	isActive_ = true;

	// 全てのスプライトを初期化
	for (int i = 0; i < 10; i++) {
		isSpritesActive_[i] = false;
		spritePosition_[i] = {position_.x + ((size_.x + lineSpace_) * i), position_.y};
		sprites_[i].reset(Sprite::Create(TextureManager::Load("./Engine/Resource/Samples/Texture", "NumberSheets.png"), &spritePosition_[i], &size_, &color_, {0.5f, 0.5f}));
		sprites_[i]->SetTextureRect({ 0.0f, 0.0f }, numberTextureSize_);
	}

	// 中央ぞろえ無効
	isCentered_ = false;
	isDisplayPercent_ = false;
}

void Counter::Initialize(uint32_t numberSheets, Vector2 numberTextureSize, int* num, Vector2 position, Vector2 size, float lineSpace)
{
	// 参照値取得
	intNum_ = num;
	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 行間設定
	lineSpace_ = lineSpace;
	
	// 色設定
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };

	// 番号のテクスチャサイズ指定
	numberTextureSize_ = numberTextureSize;

	// 表示
	isActive_ = true;

	// 全てのスプライトを初期化
	for (int i = 0; i < 10; i++) {
		isSpritesActive_[i] = false;
		spritePosition_[i] = { position_.x + ((size_.x + lineSpace_) * i), position_.y };
		sprites_[i].reset(Sprite::Create(numberSheets, &spritePosition_[i], &size_, &color_, { 0.5f, 0.5f }));
		sprites_[i]->SetTextureRect({ 0.0f, 0.0f }, numberTextureSize_);
	}

	// 中央ぞろえ無効
	isCentered_ = false;
	isDisplayPercent_ = false;
}

void Counter::Update()
{

	if (*intNum_ > 999999999)
		*intNum_ = 999999999;

	// 参照値を文字列に変換
	std::string countNum = std::to_string(*intNum_);
	int numLength = (int)countNum.length();
	if (isDisplayPercent_)
		numLength++;
	// 変換した参照値を元にスプライトを変更
	for (int i = 0; i < numLength; i++) {
		isSpritesActive_[i] = true;
		std::string count = countNum.substr(i, 1);
		if (count != "-") {
			int c = atoi(count.c_str());
			sprites_[i]->SetTextureRect({ (numberTextureSize_.x * (float)c), 0.0f }, numberTextureSize_);
		}
		else {
			sprites_[i]->SetTextureRect({ (numberTextureSize_.x * 10.0f), 0.0f }, numberTextureSize_);
		}

		if (isDisplayPercent_ && i == numLength - 1) {
			sprites_[i]->SetTextureRect({ (numberTextureSize_.x * 12.0f), 0.0f }, numberTextureSize_);
		}

		// スプライトの座標を設定
		if(isCentered_)
			spritePosition_[i] = { position_.x + (((size_.x + lineSpace_) * i) - ((size_.x / 2.0f + lineSpace_ / 2.0f) * numLength)), position_.y };
		else
			spritePosition_[i] = { position_.x + ((size_.x + lineSpace_) * i), position_.y };
	}

	for (int i = numLength; i < 10; i++)
		isSpritesActive_[i] = false;
}

void Counter::Draw()
{
	// 全てのスプライトを描画
	if(isActive_)
		for (int i = 0; i < 10; i++)
			if (isSpritesActive_[i])
				sprites_[i]->Draw();
		
}

void Counter::SetTextureHandle(uint32_t numberSheets, Vector2 numberTextureSize)
{
	// 全てのスプライトのナンバーシートを変更
	for (int i = 0; i < 10; i++)
		sprites_[i]->SetTextureHandle(numberSheets);
	numberTextureSize_ = numberTextureSize;
}
