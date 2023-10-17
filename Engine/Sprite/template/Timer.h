#pragma once
#include "../Sprite.h"
#include "Counter.h"
#include <string>

/// <summary>
/// タイマーテンプレート(99::99::59まで描画)
/// </summary>
class Timer
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="num">参照値(フレーム)</param>
	/// <param name="position">開始座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="lineSpace">行間</param>
	/// <param name="drawType">描画タイプ</param>
	void Initialize(int* num, Vector2 position, Vector2 size, float lineSpace, uint8_t drawType = 0b111);

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="numberSheets">番号テクスチャシート</param>
	/// <param name="numberTextureSize">1つ1つの番号テクスチャサイズ</param>
	/// <param name="num">参照値(フレーム)</param>
	/// /// <param name="position">開始座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="lineSpace">行間</param>
	/// <param name="drawType">行間</param>
	void Initialize(uint32_t numberSheets, Vector2 numberTextureSize, int* num, Vector2 position, Vector2 size, float lineSpace, uint8_t drawType = 0b111);

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
	// 行間
	float lineSpace_;

	// 表示トリガー
	bool isActive_;

private: // メンバ変数

	// 参照値格納用
	int* num_ = nullptr;

	// 時間格納用
	int timers_[3];

	// 描画タイプ
	// 0x111 ... 時間, 分、 秒全て
	// それぞれ 時間 分 秒
	uint8_t drawType_;

	// 1つ1つの番号テクスチャサイズ
	Vector2 numberTextureSize_;

	// 時間、分、秒を描画するスプライト
	std::unique_ptr<Counter> counters_[3];

};
