#pragma once
#include "../BaseStage.h"
#include "StageList.h"

/// <summary>
/// ゲームシーン
/// </summary>
class DebugStage final : public BaseStage {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize(const StageInfo& info) override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

public:

	// デバッグウィンドウを出す
	void DebugGUI();

private: // メンバ変数

	// 情報を入れるかどうか
	bool isApplyInfo_ = true;

};
