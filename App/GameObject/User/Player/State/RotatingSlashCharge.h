#pragma once
#include "IState.h"

/// <summary>
/// 回転斬りのチャージ状態
/// </summary>
class RotatingSlashCharge : public IState
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGuiの表示関数
	/// </summary>
	void DisplayImGui() override;

private: // メンバ変数

	// 回転切りのチャージが終わっているかフラグ
	bool isFinishedCharge_ = false;

	// 回転斬りチャージ時間の閾値
	const float chargeTimeThreshold_ = 0.6f;
};

