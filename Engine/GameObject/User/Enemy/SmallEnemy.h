#pragma once
#include "IEnemy.h"

/// <summary>
/// 小さい敵
/// </summary>
class SmallEnemy : public IEnemy
{
public: // メンバ関数

	/// <summary>
	/// 固有初期化関数
	/// </summary>
	void SuccessorInit() override;

	/// <summary>
	/// 固有更新関数
	/// </summary>
	void SuccessorUpdate() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	/// <summary>
	/// リセット関数
	/// </summary>
	void Reset() override;

	/// <summary>
	/// パラメータのセーブ関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	void SetParameter(const std::string& levelName, const std::string& enemyName) override;

	/// <summary>
	/// パラメータのロード関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	void ApplyParameter(const std::string& levelName, const std::string& enemyName) override;

private: // メンバ関数

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;

private:

	// パーツごとのトランスフォーム
	WorldTransform bodyTransform_; // 身体
	WorldTransform wingTransform_L_; // 左羽
	WorldTransform wingTransform_R_; // 右羽
};
