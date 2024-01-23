#pragma once
#include "IEnemyState.h"
class SingleAtackState:public IEnemyState
{
public:
	SingleAtackState() { name_ = "SingleAtack"; }
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

