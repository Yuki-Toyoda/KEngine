#pragma once
#include "IEnemyState.h"

/// <summary>
/// 敵の死亡アニメーションの状態
/// </summary>
class EnemyDead : public IEnemyState
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

};

