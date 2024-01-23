#pragma once
#include"../../BaseObject.h"
#include "../InputManager/InputManager.h"
#include "../../../Utility/Timer/DeltaTimer.h"
#include "../Ground/Ground.h"
#include "State/PlayerStateList.h"

// クラスの前方宣言
class PlayerAnimManager;

/// <summary>
/// プレイヤークラス
/// </summary>
class Player:public BaseObject
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

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider)override;

	/// <summary>
	/// 地面クラスセッター
	/// </summary>
	/// <param name="ground">地面</param>
	void SetGround(Ground* ground) { ground_ = ground; }

	/// <summary>
	/// 速度セッター
	/// </summary>
	/// <param name="velo">速度</param>
	void SetVelocity(Vector3 velo) { velocity_ = velo; }
	//減速率
	float decelerationRate = 0.6f;

	/// <summary>
	/// 引数で指定した行動状態に変更する関数
	/// </summary>
	/// <param name="newState">新しい行動状態</param>
	void ChangeState(std::unique_ptr<IPlayerState> newState);

	/// <summary>
	/// 攻撃関数
	/// </summary>
	void Atack();

public: // アクセッサ等

	/// <summary>
	/// 破片の吸収数ゲッター
	/// </summary>
	/// <returns>破片の吸収数</returns>
	int GetAbsorptionCount() { return absorptionCount_; }
	/// <summary>
	/// 破片吸収数リセット関数
	/// </summary>
	void ResetAbsorptionCount() { absorptionCount_ = 0; }

	/// <summary>
	/// 攻撃状態ゲッター
	/// </summary>
	/// <returns>攻撃しているか</returns>
	bool GetIsAtack() { return isAtack_; }
	/// <summary>
	/// 攻撃状態セッター
	/// </summary>
	/// <param name="Flag">設定する攻撃状態</param>
	/// <returns>設定した攻撃状態</returns>
	bool SetIsAtack(bool Flag) { return isAtack_ = Flag; }

	/// <summary>
	/// 攻撃力ゲッター
	/// </summary>
	/// <returns>攻撃力</returns>
	float GetAtackPower() { return atackPower_; }

	/// <summary>
	/// プレイヤーアニメーションマネージャ
	/// </summary>
	/// <param name="pam">プレイヤーアニメーションマネージャ</param>
	void SetPlayerAnimManager(PlayerAnimManager* pam) { pam_ = pam; }

	/// <summary>
	/// 加速度ゲッター
	/// </summary>
	/// <returns>加速度</returns>
	float GetMoveAcceleration() { return moveAcceleration_; }
	/// <summary>
	/// 最大加速度ゲッター
	/// </summary>
	/// <returns>最大加速度</returns>
	float GetMaxMoveAcceleration() { return kMaxMoveAcceleration_; }
	
	/// <summary>
	/// 減衰加速度ゲッター
	/// </summary>
	/// <returns>減衰加速度</returns>
	float GetDecayAcceleration() { return decayAcceleration_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// ダメージ処理関数
	/// </summary>
	void Damage();

	/// <summary>
	/// 移動時の減速処理関数
	/// </summary>
	void SubtractVelocity();

private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動加速度
	float moveAcceleration_ = 0.01f;
	// 移動加速度最大値
	float kMaxMoveAcceleration_ = 0.2f;

	// 減衰速度
	float decayAcceleration_ = 0.01f;
	// 減衰速度最大値
	float kMaxDecayAcceleration_ = 0.2f;

	// 一フレーム前のポジション
	Vector3 prevPos_;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;

	//吸収した数
	int absorptionCount_;
	//吸収したときのScaleの変化量
	float scaleForce_ = 0.1f;

	// HP
	int hitPoint_;
	// ダメージクールタイム用タイマー
	KLib::DeltaTimer hitCollTimer_;
	// ダメージをくらう時のクールタイム
	float hitCoolTime_=2.0f;
	// ダメージ後のスタン秒数
	float damageStanTime_ = 1.0f;

	// 地面クラス
	Ground* ground_ = nullptr;

	// 攻撃倍率
	float atackForce_ = 1.0f;
	// 攻撃力
	float atackPower_;
	//攻撃しているかどうか
	bool isAtack_;
	// 行動状態クラス
	std::unique_ptr<IPlayerState> state_;

	// プレイヤーアニメーションマネージャ
	PlayerAnimManager* pam_ = nullptr;
};

