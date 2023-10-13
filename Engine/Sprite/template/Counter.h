#pragma once
#include "../Sprite.h"
#include <string>

/// <summary>
/// 10桁までの整数型を表示するカウンタークラス
/// </summary>
class Counter
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="num">参照値</param>
	/// <param name="position">開始座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="lineSpace">行間</param>
	void Initialize(int* num, Vector2 position, Vector2 size, float lineSpace);

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="numberSheets">番号テクスチャシート</param>
	/// <param name="num">参照値</param>
	/// /// <param name="position">開始座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="lineSpace">行間</param>
	void Initialize(uint32_t numberSheets, int* num, Vector2 position, Vector2 size, float lineSpace);

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

	// 参照値
	int* num_ = nullptr;

	// スプライト表示トリガー
	bool isSpritesActive_[10];
	// スプライト
	std::unique_ptr<Sprite> sprites_[10];

};

