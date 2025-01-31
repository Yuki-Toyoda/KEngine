#pragma once
#include "Engine/Input/Input.h" 
#include "App/GameObject/User/Player/Combo/ComboManager.h"
#include "App/GameObject/User/Player/State/StateList.h"

// クラスの前方宣言
class Player;
class Input;

/// <summary>
/// プレイヤーの攻撃関連処理の管理クラス
/// </summary>
class AttackManager
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="player">プレイヤー本体</param>
	AttackManager(Player* player) { player_ = player; }
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AttackManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// プレイヤーの更新が終わった後に呼び出す更新関数
	/// </summary>
	void PostUpdate();

	/// <summary>
	/// ImGuiの表示関数
	/// </summary>
	void DisplayImGui();

private: // 機能関数群

	/// <summary>
	/// 軌跡関連処理
	/// </summary>
	void TrailUpdate();

	/// <summary>
	/// カウンター関連処理
	/// </summary>
	void CounterUpdate();

	/// <summary>
	/// 攻撃開始処理
	/// </summary>
	void AttackStartUpdate();

	/// <summary>
	/// 回転方向の補正処理
	/// </summary>
	void RotateCorrectUpdate();

public: // アクセッサ等

	/// <summary>
	/// コンボマネージャーゲッター
	/// </summary>
	/// <returns>コンボマネージャー</returns>
	ComboManager* GetComboManager() { return &comboManager_; }

	/// <summary>
	/// 攻撃の命中状態セッター
	/// </summary>
	/// <param name="isHit">命中状態</param>
	void SetIsHit(const bool isHit) { isHit_ = isHit; }
	/// <summary>
	/// 攻撃の命中状態ゲッター
	/// </summary>
	/// <returns>命中状態</returns>
	bool GetIsHit() { return isHit_; }

	/// <summary>
	/// 攻撃状態セッター
	/// </summary>
	/// <param name="isAttacking">攻撃状態</param>
	void SetIsAttacking(const bool isAttacking) { isAttacking_ = isAttacking; }
	/// <summary>
	/// 攻撃状態ゲッター
	/// </summary>
	/// <returns>攻撃状態</returns>
	bool GetIsAttacking() { return isAttacking_; }

	/// <summary>
	/// 敵への補正有効状態のゲッター
	/// </summary>
	/// <returns>敵に補正しているか</returns>
	bool GetIsCorrectingToEnemy() { return isCorrectingToEnemy_; }

	/// <summary>
	/// カウンター可能状態のセッター
	/// </summary>
	/// <param name="isCan">カウンター可能状態</param>
	void SetIsCanCounter(const bool isCan) { isCanCounter_ = isCan; }
	/// <summary>
	/// カウンター可能状態のゲッター
	/// </summary>
	/// <returns>カウンター可能状態</returns>
	bool GetIsCanCounter() { return isCanCounter_; }

private: // メンバ変数

	// プレイヤー本体
	Player* player_ = nullptr;
	// 入力管理クラス
	Input* input_ = nullptr;

	// コンボ管理クラス
	ComboManager comboManager_{};

	// 攻撃可能か
	bool canAttack_ = true;
	// 攻撃中か
	bool isAttacking_ = false;
	// 攻撃が命中したか
	bool isHit_ = false;

	// 現在敵に向けて補正をかけているのか
	bool isCorrectingToEnemy_ = false;
	// 敵がいる向きを保存
	Vector3 enemyDirection_{};
	// 攻撃時に回転補正をかける場合の距離
	const float correctDirectionDistance = 5.0f;
	// 回転補正を終了する閾値
	const float angleCorrectThreshold = 0.05f;
	// 敵がいる方向に補正する際の速度
	const float toEnemyCorrectSpeed = 0.25f;

	// カウンター可能か
	bool isCanCounter_ = false;

	// 軌跡補間速度
	const float startAppearTrailCorrectSpeed = 0.2f;
	const float endAppearTrailCorrectSpeed = 0.3f;
	// 軌跡透明度
	const float maxTrailAlpha = 1.0f;
	const float minTrailAlpha = 0.0f;
	// 軌跡の強制透過閾値
	const float trailAlphaThresold = 0.01f;
};

