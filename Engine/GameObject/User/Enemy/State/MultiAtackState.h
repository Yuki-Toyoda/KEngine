#pragma once
#include "IEnemyState.h"

class MultiAtackState: public IEnemyState
{
public:
	MultiAtackState() { name_ = "MultiAtack"; }
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
	//タイマー
	KLib::DeltaTimer atackTimer_;
	//攻撃のクールタイム
	float coolTime_ = 1.0f;
	//現在の攻撃回数
	int atackCount_=0;
	//何回攻撃するか
	int kMaxCount_=5;
	
	
};

