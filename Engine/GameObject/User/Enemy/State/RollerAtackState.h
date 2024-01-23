#pragma once
#include "IEnemyState.h"
#include "../Atack/Roller.h"
class RollerAtackState:public IEnemyState
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

private: // メンバ変数

	
};

