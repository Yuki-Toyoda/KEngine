#pragma once
#include "IPlayerState.h"

/// <summary>
/// 攻撃行動
/// </summary>
class AtackState:public IPlayerState
{
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <returns></returns>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public:

};

