#pragma once
#include "IState.h"
#include "../../../../Utility/KLib.h"

/// <summary>
/// ダメージを喰らった時の処理
/// </summary>
class Damage : public IState
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

	Vector3 velocity_;

};

