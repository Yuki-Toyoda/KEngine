#pragma once
#include "IEnemyState.h"
#include "Engine/Utility/KLib.h"

/// <summary>
/// 敵のダウン状態のクラス
/// </summary>
class EnemyDown : public IEnemyState
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

	// 最高高度への補間スピード
	const float klerpSpeed_ = 0.015f;
	// 高度
	const float kHeight_ = 0.0f;

	// ダウン秒数
	const float kMaxDownTime_ = 5.0f;

	// 落下状態へ移行するアニメーションの進行度割合
	const float transitionFallStateProgress_ = 0.35f;
	// 落下させるか
	bool isFalling_ = false;

	// ダウン秒数タイマー
	KLib::DeltaTimer timer_;
};

