#pragma once
#include "../BaseStage.h"
#include "StageList.h"

/// <summary>
/// ゲームシーン
/// </summary>
class SecondeStage final : public BaseStage {
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

};
