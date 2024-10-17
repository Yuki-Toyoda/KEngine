#pragma once
#include "IState.h"

/// <summary>
/// 攻撃状態クラス
/// </summary>
class Attack : public IState
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

	// 回転斬りのチャージへの移行閾値
	const float chargeTransitionThreshold_ = 0.6f;
	const float slashTransitionThreshold_ = 0.8f;
};

