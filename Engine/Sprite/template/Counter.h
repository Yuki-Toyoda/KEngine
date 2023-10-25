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
	/// <param name="numberTextureSize">1つ1つの番号テクスチャサイズ</param>
	/// <param name="num">参照値</param>
	/// /// <param name="position">開始座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="lineSpace">行間</param>
	void Initialize(uint32_t numberSheets, Vector2 numberTextureSize, int* num, Vector2 position, Vector2 size, float lineSpace);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // アクセッサ等

	/// <summary>
	/// 参照値(int*)のセッター
	/// </summary>
	/// <param name="num">参照値</param>
	void SetNum(int* num) { intNum_ = num; }

	/// <summary>
	/// 番号テクスチャセッター
	/// </summary>
	/// <param name="numberSheets">設定したい番号シート</param>
	/// <param name="numberTextureSize">1つ1つの番号テクスチャサイズ</param>
	void SetTextureHandle(uint32_t numberSheets, Vector2 numberTextureSize);

	/// <summary>
	/// 中央ぞろえセッター
	/// </summary>
	/// <param name="isCenterd">中央ぞろえにするか</param>
	void SetIsCentered(bool isCenterd) { isCentered_ = isCenterd; }

	/// <summary>
	/// ％表示セッター
	/// </summary>
	/// <param name="isDisplay">％表示をするか</param>
	void SetIsDispayPercent(bool isDisplay) { isDisplayPercent_ = isDisplay; }

public: // パブリックなメンバ変数

	// 座標
	Vector2 position_;
	// 大きさ
	Vector2 size_;
	// 行間
	float lineSpace_;
	// 色
	Vector4 color_;

	// 表示トリガー
	bool isActive_;

private: // メンバ変数

	// 参照値
	int* intNum_ = nullptr;

	// 1つ1つの番号テクスチャサイズ
	Vector2 numberTextureSize_;

	// スプライトごとの座標
	Vector2 spritePosition_[10];

	// スプライト表示トリガー
	bool isSpritesActive_[10];
	// スプライト
	std::unique_ptr<Sprite> sprites_[10];

	// 中央ぞろえトリガー
	bool isCentered_;
	// ％表示トリガー
	bool isDisplayPercent_;

};

