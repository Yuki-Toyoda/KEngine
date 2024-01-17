#pragma once
#include <list>
#include "../IEnemy.h"
#include "../../../GameObjectManager.h"

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

	/// <summary>
	/// 敵生成関数
	/// </summary>
	/// <typeparam name="SelectEnemy">生成する敵の型(IEnemy継承クラスのみ)</typeparam>
	/// <param name="name">生成する敵名</param>
	/// <param name="tag">敵のタグ</param>
	/// <returns>生成された敵</returns>
	template<IsIEnemy SelectEnemy>
	inline IEnemy* CreataeEnemy(const std::string& name, BaseObject::Tag tag = BaseObject::TagEnemy) {
		// インスタンス生成
		SelectEnemy* newEnemy = objManager_->CreateInstance<SelectEnemy>(name, tag);
	}

private: // メンバ変数

	// ゲームオブジェクトマネージャー
	GameObjectManager* objManager_ = nullptr;

	// 敵のリスト
	std::list<IEnemy*> enemies_;

};

