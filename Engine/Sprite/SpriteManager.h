#pragma once
#include <list>
#include <memory>
#include "Sprite.h"

/// <summary>
/// スプライトのマネージャ
/// </summary>
class SpriteManager
{
private: // コンストラクタ、デストラクタ等

	// シングルトンパターンの設定
	SpriteManager() = default;
	~SpriteManager() = default;
	SpriteManager(const SpriteManager&) = delete;
	const SpriteManager& operator=(const SpriteManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SpriteManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// スプライト生成関数
	/// </summary>
	/// <param name="name">スプライト名</param>
	Sprite* Create(const std::string& name);

	/// <summary>
	/// スプライト生成関数
	/// </summary>
	/// <param name="name">スプライト名</param>
	/// <param name="position">初期座標</param>
	/// <param name="size">大きさ</param>
	/// <param name="texture">テクスチャ</param>
	Sprite* Create(const std::string& name, const Vector2 position, const Vector2& size, Texture* texture);

private: // メンバ変数

	// スプライトリスト
	std::list<std::unique_ptr<Sprite>> sprites_;

};

