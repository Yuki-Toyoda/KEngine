#pragma once
#include "BaseScene.h"
#include "ISceneFactory.h"

/// <summary>
/// シーンマネージャー
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
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui();

public: // アクセッサ等

	/// <summary>
	/// シーンファクトリーセッター
	/// </summary>
	/// <param name="sceneFactory">シーンファクトリー</param>
	void SetSceneFactory(std::unique_ptr<ISceneFactory> sceneFacotory) { sceneFactory_ = std::move(sceneFacotory); }

	/// <summary>
	/// シーン切り替え関数
	/// </summary>
	/// <param name="name">切り替えシーン名</param>
	void ChangeScene(const std::string& name);

	/// <summary>
	/// 現在シーンのゲッター
	/// </summary>
	/// <returns>現在のシーン</returns>
	BaseScene* GetCurrentScene() { return currentScene_; }

private: // メンバ変数

	// 現在のシーン
	BaseScene* currentScene_ = nullptr;
	// 次のシーン
	BaseScene* nextScene_ = nullptr;

	// シーンファクトリー
	std::unique_ptr<ISceneFactory> sceneFactory_;
};

