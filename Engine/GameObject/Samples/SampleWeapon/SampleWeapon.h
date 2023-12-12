#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 武器のサンプル
/// </summary>
class SampleWeapon : public BaseObject
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

	/// <summary>
	/// 判定の追従対象セッター
	/// </summary>
	/// <param name="target">追従対象</param>
	void SetTarget(const WorldTransform* target) { target_ = target; }

	/// <summary>
	/// 攻撃状態セッター
	/// </summary>
	/// <param name="enable">攻撃状態</param>
	void SetIsAttack(bool enable) { isAttack_ = enable; }

private: // メンバ変数

	// 追従対象のワールド座標
	const WorldTransform* target_ = nullptr;
	// 当たり判定ワールド座標
	WorldTransform colliderTransform_;
	// 攻撃状態フラグ
	bool isAttack_;

};

