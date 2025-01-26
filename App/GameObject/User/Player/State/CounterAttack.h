#pragma once
#include "IState.h"
#include "Engine/Utility/KLib.h"

// クラスの前方宣言
class Enemy;

/// <summary>
/// カウンター攻撃
/// </summary>
class CounterAttack : public IState
{
private: // プライベートなサブクラス

	/// <summary>
	/// カウンター攻撃進捗列挙子
	/// </summary>
	enum CounterAttackProgress {
		MOVE,
		ATTACK,
		END
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

private: // 機能関数群

	/// <summary>
	/// 移動更新関数
	/// </summary>
	void MoveUpdate();

	/// <summary>
	/// 攻撃更新関数
	/// </summary>
	void AttackUpdate();

	/// <summary>
	/// 終了更新関数
	/// </summary>
	void EndUpdate();

	/// <summary>
	/// Catmull=Spline曲線関数
	/// </summary>
	/// <param name="p0">制御点</param>
	/// <param name="p1">開始地点</param>
	/// <param name="p2">補間点</param>
	/// <param name="p3">終了地点</param>
	/// <param name="t">t (0 ~ 1)</param>
	/// <returns>曲線補間座標</returns>
	Vector3 CatmullRomSpline(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, const float t);

private: // メンバ変数

	// 移動時間
	const float moveTime = 0.3f;
	// 終了時間
	const float endTime = 0.35f;

	// 敵
	Enemy* enemy_ = nullptr;

	// 補間点
	Vector3 startControlPos_{};
	Vector3 endControlPos_{};
	// 補間開始座標
	Vector3 startPos_{};
	// 目標点
	Vector3 targetPos_{};
	// 補間終了座標
	Vector3 endPos_{};

	// 進捗管理用
	int progress_ = MOVE;

	// 演出用タイマー
	KLib::DeltaTimer stagingTimer_{};
};

