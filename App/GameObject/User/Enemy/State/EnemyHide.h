#pragma once
#include "IEnemyState.h"
#include "Engine/Utility/Timer/DeltaTimer.h"

/// <summary>
/// 一定時間隠れ、プレイヤーから距離をとる行動
/// </summary>
class EnemyHide : public IEnemyState
{
private: // プライベートなサブクラス

	/// <summary>
	/// 隠れ演出進捗管理用列挙子
	/// </summary>
	enum HideProcess
	{
		HIDESTART,
		HIDING,
		HIDEEND
	};

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

	// 隠れ演出開始秒数
	const float hideStartTime = 2.5f;
	// 隠れ演出中秒数
	const float hideTime = 1.5f;
	// 隠れ演出終了秒数
	const float hideEndTime = 1.0f;

	// 演出進捗
	int progress_ = HIDESTART;

	// プレイヤー座標保存用
	Vector3 playerPosition_{};
	// 減算保存用
	Vector3 sub_{};

	// ディゾルブ時のエッジ色
	const Vector3 dissolveEdgeColor_ = { 0.0f, 1.0f, 0.75f };
	// ディゾルブを光らせる閾値
	const float dissolveEdgeThreshold_ = 0.025f;

	// 演出用タイマー
	KLib::DeltaTimer stagingTimer_{};
};

