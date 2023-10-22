#pragma once
#include "../BaseScene.h"
#include "SceneList.h"
#include "../../Stage/user/StageList.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene final : public BaseScene {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

	// ステージマネージャー
	StageManager* stageManager_ = nullptr;

	// ゲームマネージャー
	GameManagerObject* gameManager_ = nullptr;
};
