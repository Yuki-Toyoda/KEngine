#pragma once
#include "IState.h"

class RotatingSlash : public IState
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

	// 回転切りに入ったフラグ
	bool isRotateSlashing_ = false;

};

