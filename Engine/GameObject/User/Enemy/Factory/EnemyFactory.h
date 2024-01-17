#pragma once
#include "../IEnemy.h"
#include "../../../GameObjectManager.h"

/// <summary>
/// 敵の生成を行うファクトリークラス
/// </summary>
class EnemyFactory
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効
	EnemyFactory() = default;
	~EnemyFactory() = default;
	EnemyFactory(const EnemyFactory& variable) = default;
	EnemyFactory& operator=(const EnemyFactory& variable);

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>インスタンス</returns>
	static EnemyFactory* GetInstance() {
		static EnemyFactory instance;
		return &instance;
	}

	/// <summary>
	/// 敵生成関数
	/// </summary>
	/// <typeparam name="SelectEnemy">生成する敵の型(IEnemy継承クラスのみ)</typeparam>
	/// <param name="name">生成する敵名</param>
	/// <param name="tag">敵のタグ</param>
	/// <param name="translate">生成座標</param>
	/// <returns>生成された敵</returns>
	template<IsIEnemy SelectEnemy>
	inline IEnemy* CreataeEnemy(const std::string& name, BaseObject::Tag tag = BaseObject::TagEnemy, const Vector3& translate) {
		// オブジェクトマネージャが取得できていない場合
		if (objManager_ == nullptr) {
			// ゲームオブジェクトマネージャーの取得
			objManager_ = GameObjectManager::GetInstance();
		}
		
		// 新しい敵のインスタンスを生成
		SelectEnemy* newEnemy = objManager_->CreateInstance<SelectEnemy>(name, tag);
		// 生成座標に移動
		newEnemy->transform_.translate_ = translate;

		// 生成インスタンスを返す
		return newEnemy;
	}

private: // メンバ変数

	// ゲームオブジェクトマネージャー
	GameObjectManager* objManager_ = nullptr;

};

