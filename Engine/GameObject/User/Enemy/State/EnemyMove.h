#pragma once
#include "IEnemyState.h"
#include "../../../../Utility/Timer/DeltaTimer.h"
#include "../../../../Utility/Lerp/Lerp.h"
#include "../../../../Utility/Easing/Easing.h"

/// <summary>
/// プレイヤーが近づきすぎた際にプレイヤーから離れるステート
/// </summary>
class EnemyMove : public IEnemyState
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

	// 移動時間タイマー
	KLib::DeltaTimer moveTimer_;

	// 移動元座標
	Vector3 prevPosition_ = {};
	// 移動先座標
	Vector3 movePosition_ = {};

};

