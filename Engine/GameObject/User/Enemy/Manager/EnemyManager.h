#pragma once
#include <list>
#include "../IEnemy.h"

/// <summary>
/// 敵のマネージャー
/// </summary>
class EnemyManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効
	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager& variable) = default;
	EnemyManager& operator=(const EnemyManager& variable);

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static EnemyManager* GetInstance() {
		static EnemyManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

private: // メンバ変数

	// 敵のリスト
	std::list<IEnemy*> enemies_;

};

