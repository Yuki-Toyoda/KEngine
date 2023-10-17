#include "Timer.h"
#include "../../Resource/Texture/TextureManager.h"

// 親子タイプのフラグ
const int FLAG_HOUR = 0b100; // 時間用フラグ
const int FLAG_MINUTE = 0b010; // 分用フラグ
const int FLAG_SECOND = 0b001; // 秒用フラグ

void Timer::Initialize(int* num, Vector2 position, Vector2 size, float lineSpace, uint8_t drawType)
{
	// 参照値取得
	num_ = num;

	// 時間を求める
	timers_[0] = *num_ / 21600;
	// 分を求める
	timers_[1] = *num_ / 3600;
	// 秒を求める
	timers_[2] = (*num_ % 3600) / 60;

	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 行間設定
	lineSpace_ = lineSpace;
	// 描画タイプ設定
	drawType_ = drawType;

	// 番号のテクスチャサイズ指定
	numberTextureSize_ = { 512.0f, 512.0f };

	// 表示
	isActive_ = true;

	// 全てのカウンターを初期化
	for (int i = 0; i < 3; i++) {
		counters_[i] = std::make_unique<Counter>();
		Vector2 offsetPosition = { position_.x + (((size_.x * 2) + lineSpace_) * i), position_.y };
		counters_[i]->Initialize(&timers_[i], offsetPosition, size, lineSpace);
	}
	
}

void Timer::Initialize(uint32_t numberSheets, Vector2 numberTextureSize, int* num, Vector2 position, Vector2 size, float lineSpace, uint8_t drawType)
{
	// 参照値取得
	num_ = num;
	// 座標設定
	position_ = position;
	// 大きさ設定
	size_ = size;
	// 行間設定
	lineSpace_ = lineSpace;
	// 描画タイプ設定
	drawType_ = drawType;

	// 番号のテクスチャサイズ指定
	numberTextureSize_ = numberTextureSize;

	// 表示
	isActive_ = true;

	// 全てのカウンターを初期化
	for (int i = 0; i < 3; i++) {
		counters_[i] = std::make_unique<Counter>();
		Vector2 offsetPosition = { position_.x + (((size_.x * 2) + lineSpace_) * i), position_.y };
		counters_[i]->Initialize(numberSheets, numberTextureSize, &timers_[i], offsetPosition, size, lineSpace);
	}
	
}

void Timer::Update()
{

	// 時間を求める
	timers_[0] = *num_ / 21600;
	// 分を求める
	timers_[1] = *num_ / 3600;
	// 秒を求める
	timers_[2] = (*num_ % 3600) / 60;

	// 全てのカウンターを更新
	for (int i = 0; i < 3; i++) {
		Vector2 offsetPosition = { position_.x + ((size_.x + lineSpace_) * i), position_.y };
		counters_[i]->position_ = offsetPosition;
		counters_[i]->Update();
	}
}

void Timer::Draw()
{
	// 全てのカウンターを描画
	for (int i = 0; i < 3; i++) {
		counters_[i]->Draw();
	}
}
