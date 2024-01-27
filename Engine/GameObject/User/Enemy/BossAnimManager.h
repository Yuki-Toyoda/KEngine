#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Animation/AnimationManager.h"

// クラスの前方宣言
class Boss;

/// <summary>
/// ボスのアニメーションマネージャー
/// </summary>
class BossAnimManager : public BaseObject
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
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">パラメータ名</param>
	void CreateParameter(const std::string& name);

	/// <summary>
	/// アニメーション生成関数
	/// </summary>
	void CreateAnimation();

public: // アクセッサ等

	/// <summary>
	/// ボスのセッター
	/// </summary>
	/// <param name="boss">ボスの実体</param>
	void SetBoss(Boss* boss);

private: // メンバ変数

	// アニメーションマネージャー
	AnimationManager* animManager_ = nullptr;

	// アニメーション
	Animation* anim_ = nullptr;

	// ボスの実体
	Boss* boss_ = nullptr;
	
	// パーツごとのトランスフォーム
	WorldTransform bodyTransform_; // 身体
	WorldTransform armTransform_L_; // 左腕
	WorldTransform armTransform_R_; // 左腕
	WorldTransform footTransform_L_; // 左腕
	WorldTransform footTransform_R_; // 左腕

};

