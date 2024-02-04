#pragma once
#include"../State/IEnemyState.h"
#include"../../../../Utility/Timer/DeltaTimer.h"
class TutrialState:public IEnemyState
{
public:
	TutrialState() { name_ = "Tutrial"; }
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

	// 野菜配列
	std::vector<Meteor*> meteors_;

	// 野菜の落下トリガー
	bool isFall_ = false;
	//攻撃用タイマー
	KLib::DeltaTimer timer_;
	//攻撃のクールタイム
	float coolTime_;
};

