#pragma once
#include "IEnemyState.h"

/// <summary>
/// 待機状態クラス
/// </summary>
class EnemyRoot : public IEnemyState
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

	// 攻撃可能高度
	const float kCanAttackHeight_ = 2.5f;
	// 最高高度
	const float kHeight_ = 3.0f;
	// 最高高度への補間スピード
	const float klerpSpeed_ = 0.01f;

	// 待機アニメーションへの遷移秒数
	const float idleAnimTransitionTime_ = 0.25f;

};

