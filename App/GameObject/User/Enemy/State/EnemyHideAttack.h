#pragma once
#include "IEnemyState.h"
#include "Engine/Utility/Timer/DeltaTimer.h"

/// <summary>
/// プレイヤーの背後にテレポート、攻撃する
/// </summary>
class EnemyHideAttack : public IEnemyState
{
private: // プライベートなサブクラス

	/// <summary>
	/// 背面攻撃進捗列挙子
	/// </summary>
	enum HideAttakProcess {
		HIDESTART,
		HIDING,
		APPEAR,
		WAIT,
		ATTACK
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

private: // 各状態での更新関数

	/// <summary>
	/// ハイド開始関係更新関数
	/// </summary>
	void HideStartUpdate();

	/// <summary>
	/// ハイド中関係更新関数
	/// </summary>
	void HidingUpdate();

	/// <summary>
	/// 出現関係更新関数
	/// </summary>
	void AppearUpdate();

	/// <summary>
	/// 待機関係更新関数
	/// </summary>
	void WaitUpdate();

	/// <summary>
	/// 攻撃関係更新関数
	/// </summary>
	void AttackUpdate();

private: // メンバ変数

	// ハイド開始時間
	const float hideStartTime = 2.5f;
	// 隠れている時間
	const float hidingTime = 1.0f;
	// 出現にかかる秒数
	const float appearTime = 1.0f;
	// 出現後の攻撃待機時間
	const float waitTime = 0.1f;
	// 攻撃時間
	const float attackTime = 1.0f;

	// プレイヤート敵との距離オフセット
	const Vector3 kDistanceOffset = { 0.0f, 0.0f, -3.0f };

	// ディゾルブ時のエッジ色
	const Vector3 dissolveEdgeColor_ = { 0.0f, 1.0f, 0.75f };
	// ディゾルブを光らせる閾値
	const float dissolveEdgeThreshold_ = 0.025f;

	// 攻撃時の移動スピード
	const float speed = 0.5f;
	// 移動時の方向ベクトル
	Vector3 velocity_{};

	// 進捗
	int process_ = HIDESTART;

	// 演出用タイマー
	KLib::DeltaTimer stagingTimer_{};

};

