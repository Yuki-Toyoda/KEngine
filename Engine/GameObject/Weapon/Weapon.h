#pragma once
#include "../BaseObject.h"

/// <summary>
/// プレイヤーの武器
/// </summary>
class Weapon : public BaseObject
{
public: // メンバ関数

	// 固有の初期化処理
	void Initialize(std::string name, Tag tag) override;
	// 更新処理
	void Update() override;
	// 描画処理
	void Draw() override;

public: // その他関数群

	/// <summary>
	/// グローバル変数に値を追加する関数
	/// </summary>
	void AddGlobalVariables() override;

	/// <summary>
	/// グローバル変数の値を適用する関数
	/// </summary>
	void ApplyGlobalVariables() override;

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

