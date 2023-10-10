#pragma once
#include "../BaseScene.h"
#include "SceneList.h"

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

	// ゴールクラス
	std::unique_ptr<Goal> goal_;

};

