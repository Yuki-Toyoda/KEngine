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

	// 最高高度
	const float kHeight_ = 4.0f;
	// 最高高度への補間スピード
	const float klerpSpeed_ = 0.01f;

};

