#pragma once
#include "../../BaseObject.h"

// クラスの前方宣言
class Player;
class Weapon;

/// <summary>
/// プレイヤーのアニメーションマネージャ
/// </summary>
class PlayerAnimManager : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化
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
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player">プレイヤーの実体</param>
	void SetPlayer(Player* player);

private: // メンバ変数

	// プレイヤーの実体
	Player* player_;

	// パーツごとのトランスフォーム
	WorldTransform bodyTransform_; // 身体
	WorldTransform wingTransform_L_; // 左羽
	WorldTransform wingTransform_R_; // 右羽
	WorldTransform armTransform_L_; // 左腕
	WorldTransform armTransform_R_; // 右腕
	WorldTransform footTransform_L_; // 左腕
	WorldTransform footTransform_R_; // 右腕

};

