#pragma once
#include "IEnemy.h"

/// <summary>
/// 小さい敵
/// </summary>
class SmallEnemy : public IEnemy
{
public: // メンバ関数
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

	/// <summary>
	/// リセット関数
	/// </summary>
	virtual void Reset();

	/// <summary>
	/// パラメータのセーブ関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void SaveParameter(const std::string& levelName, const std::string& enemyName);

	/// <summary>
	/// パラメータのロード関数
	/// </summary>
	/// <param name="levelName">レベル名</param>
	/// <param name="enemyName">敵名</param>
	virtual void LoadParameter(const std::string& levelName, const std::string& enemyName);

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

