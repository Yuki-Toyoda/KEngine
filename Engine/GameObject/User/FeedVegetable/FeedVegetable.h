#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 餌を渡す際に動かす野菜
/// </summary>
class FeedVegetable : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 初期化後処理
	/// </summary>
	/// <param name="start">開始地点</param>
	/// <param name="end">終端地点</param>
	void PostInit(const Vector3& start, const Vector3& end);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

private: // メンバ変数

	// 線形補間開始地点
	Vector3 startPos_ = { 0.0f, 0.0f, 0.0f };
	// 線形補間終端地点
	Vector3 endPos_ = { 0.0f, 0.0f, 0.0f };

	// 動作のリターンフラグ
	bool isReturn_ = false;

	// 線形補間タイマー
	KLib::DeltaTimer lerpTimer_;
	// Y軸のみの線形補間タイマー
	KLib::DeltaTimer lerpTimerY_;
	// 線形補間秒数
	float lerpTime_ = 0.0f;
};

