#pragma once
#include "BaseScene.h"

/// <summary>
/// 全てのシーンの管理マネージャ
/// </summary>
class SceneManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	SceneManager() = default;
	~SceneManager() = default;
	SceneManager(const SceneManager&) = delete;
	SceneManager& operator=(const SceneManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static SceneManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

private: // メンバ変数
	
	// 現在のシーン
	BaseScene* currentScene_;

};

