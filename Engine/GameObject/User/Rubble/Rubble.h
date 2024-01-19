#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/KLib.h"

class Rubble:public BaseObject
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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

private:
	void OnCollisionEnter(Collider* collider)override;
	
private:
	// 線形補間用タイマー
	KLib::DeltaTimer lerpTimer_;

	// 遷移にかかる秒数
	float lerpTime_ = 3.0f;
};

