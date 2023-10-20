#pragma once
#include "../BaseStage.h"
#include "StageList.h"

/// <summary>
/// タイトルシーン
/// </summary>
class Stage final : public BaseStage {
public: // メンバ関数

	Stage();
	~Stage() = default;

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize(const StageInfo& info) override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

};
