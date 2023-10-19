#pragma once
#include "../BaseScene.h"
#include "SceneList.h"

/// <summary>
/// ステージ選択シーン
/// </summary>
class StageSelectScene final : public BaseScene {
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

	// タイトルマネージャー
	StageSelectManagerObject* stageSelectManager_ = nullptr;

};

