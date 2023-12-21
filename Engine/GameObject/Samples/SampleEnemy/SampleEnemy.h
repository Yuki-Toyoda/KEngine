#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 敵のサンプル
/// </summary>
class SampleEnemy : public BaseObject
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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // その他関数群

	

private: // メンバ変数

	// 階層アニメーション用トランスフォーム
	WorldTransform TireTransform_R_; // 右タイヤ
	WorldTransform TireTransform_L_; // 左タイヤ

	// 移動速度
	Vector3 velocity_;

	// 接地判定
	bool isLanding_;
	// 最大落下速度
	float kMaxFallSpeed_;
	// 現在落下速度
	float fallSpeed_;
	// 落下加速度
	float kFallAcceleration_;

	// 攻撃が当たったか
	bool isHit_;

};

