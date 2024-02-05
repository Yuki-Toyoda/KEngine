#pragma once
#include "IEnemyState.h"
#include "../Atack/Roller.h"
class RollerAtackState:public IEnemyState
{
public:
	RollerAtackState() { name_ = "Roller"; }
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

	// ローラー配列
	std::vector<Roller*> rollers_;
	
	// 転がり開始
	bool isRotateStart_ = false;

	KLib::DeltaTimer fallTimer_;

};

