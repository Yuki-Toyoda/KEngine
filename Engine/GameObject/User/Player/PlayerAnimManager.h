#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Animation/AnimationManager.h"

// クラスの前方宣言
class Player;

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

	/// <summary>
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">パラメータ名</param>
	void CreateParameter(const std::string& name);

	/// <summary>
	/// アニメーション生成関数
	/// </summary>
	void CrateAnimation();

	/// <summary>
	/// 読み込みパラメータ変更関数
	/// </summary>
	/// <param name="name">読み込みパラメータ名</param>
	/// <param name="isChanged">実行時にそのパラメータへの強制的な遷移を行うか</param>
	void ChangeParamameter(const std::string& name, const bool& isChanged = true);

	/// <summary>
	/// ダメージ関数
	/// </summary>
	/// <param name="stanTime">スタン秒数</param>
	void Damage(const float& stanTime);

public: // アクセッサ等

	/// <summary>
	/// アニメーションのゲッター
	/// </summary>
	/// <returns>アニメーション</returns>
	Animation* GetAnimation() { return anim_; }

public: // パブリックなメンバ変数

	// パーツごとのトランスフォーム
	WorldTransform bodyTransform_; // 身体

private: // メンバ変数

	// アニメーションマネージャ
	AnimationManager* animManager_ = nullptr;
	// アニメーション
	Animation* anim_ = nullptr;

	// プレイヤーの実体
	Player* player_ = nullptr;

	// ダメージ後のスタン秒数タイマー
	KLib::DeltaTimer stanTimer_;

	// 
	float imGuiAnimationTime_ = 1.0f;

};

