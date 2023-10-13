#include "Counter.h"
#include "../../Resource/Texture/TextureManager.h"

void Counter::Initialize(int* num, Vector2 position, Vector2 size, float lineSpace)
{
	// 参照値取得
	num_ = num;
	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 行間設定
	lineSpace_ = lineSpace;

	// 表示
	isActive_ = true;

	// 全てのスプライトを初期化
	for (int i = 0; i < 10; i++) {
		Vector2 offsetPosition = { position_.x + (((size_.x / 2.0f) + lineSpace_) * i), position_.y };
		sprites_[i].reset(Sprite::Create(TextureManager::Load("./Engine/Resource/Samples/Texture", "NumberSheets.png"), offsetPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
		sprites_[i]->size_ = size;
		sprites_[i]->SetTextureRect({ 0.0f, 0.0f }, { 512.0f, 512.0f });
	}
}

void Counter::Initialize(uint32_t numberSheets, int* num, Vector2 position, Vector2 size, float lineSpace)
{
	// 参照値取得
	num_ = num;
	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 行間設定
	lineSpace_ = lineSpace;

	// 表示
	isActive_ = true;

	// 全てのスプライトを初期化
	for (int i = 0; i < 10; i++) {
		isSpritesActive_[i] = false;
		Vector2 offsetPosition = { position_.x + ((size_.x + lineSpace_) * i), position_.y };
		sprites_[i].reset(Sprite::Create(numberSheets, offsetPosition, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.5f, 0.5f }));
		sprites_[i]->size_ = size;
		sprites_[i]->SetTextureRect({ 0.0f, 0.0f }, { 512.0f, 512.0f });
	}
}

void Counter::Update()
{
	// 全てのスプライトの座標と大きさを更新
	for (int i = 0; i < 10; i++) {
		Vector2 offsetPosition = { position_.x + ((size_.x + lineSpace_) * i), position_.y };
		sprites_[i]->position_ = offsetPosition;
		sprites_[i]->size_ = size_;
	}

	// 参照値を文字列に変換
	std::string countNum = std::to_string(*num_);

	// 変換した参照値を元にスプライトを変更
	for (int i = 0; i < (int)countNum.length(); i++) {
		isSpritesActive_[i] = true;
		std::string count = countNum.substr(i, 1);
		if (count != "-") {
			int c = atoi(count.c_str());
			sprites_[i]->SetTextureRect({ (512.0f * (float)c), 0.0f }, { 512.0f, 512.0f });
		}
		else
			sprites_[i]->SetTextureRect({ (512.0f * 10.0f), 0.0f }, { 512.0f, 512.0f });
	}

	for (int i = (int)countNum.length(); i < 10; i++)
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
