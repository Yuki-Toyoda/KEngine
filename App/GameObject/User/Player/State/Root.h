#pragma once
#include "IState.h"

/// <summary>
/// 待機状態行動クラス
/// </summary>
class Root : public IState
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

	// スティックのデッドゾーン
	const float deadZone_ = 0.25f;

	// 移動速度
	const float kMaxSpeed_ = 0.15f;

	const float kMaxZforcusSpeed_ = 0.1f;

	// 現在の移動速度
	Vector3 speed_ = Vector3();

	// 追従対象の目標角度
	float targetAngle_ = 0.0f;

	// 移動フラグ
	bool isMoving_ = false;

};

