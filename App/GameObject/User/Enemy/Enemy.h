#pragma once
#include "Engine/GameObject/IObject.h"
#include "Engine/Utility/Animation/AnimationManager.h"
#include "State/EnemyStateList.h"
#include "Engine/Utility/Random/Random.h"

// クラスの前方宣言
class Player;
class GameManager;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy : public IObject
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

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;
	
	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="collider">衝突したコライダー</param>
	void OnCollision(Collider* collider) override;

	/// <summary>
	/// 
	/// </summary>
	/// <param name="collider"></param>
	void OnCollisionExit(Collider* collider) override;

	/// <summary>
	/// パラメータを作成する
	/// </summary>
	/// <param name="name">作成するパラメータ</param>
	void CreateParameter(const std::string& name);

	/// <summary>
	/// 行動状態変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IEnemyState> newState);

public: // アクセッサ等

	/// <summary>
	/// ゲームマネージャーセッター
	/// </summary>
	/// <param name="gameManager">ゲームマネージャー</param>
	void SetGameManager(GameManager* gameManager) { gameManager_ = gameManager; }
	/// <summary>
	/// ゲームマネージャーゲッター
	/// </summary>
	/// <returns>ゲームマネージャー</returns>
	GameManager* GetGameManager() { return gameManager_; }

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }
	/// <summary>
	/// プレイヤーゲッター
	/// </summary>
	/// <returns>プレイヤー本体</returns>
	Player* GetPlayer() { return player_; }

	/// <summary>
	/// プレイヤー座標セッター
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	void SetPlayerPos(const WorldTransform* playerPos) { playerPos_ = playerPos; }

	/// <summary>
	/// 左腕のワールド座標ゲッター
	/// </summary>
	/// <returns>左腕のワールド座標</returns>
	Vector3 GetLeftArmPosition() { return armTransform_L_.GetWorldPos(); }

	/// <summary>
	/// 死亡状態セッター
	/// </summary>
	/// <param name="isDead">s</param>
	void SetIsDead(const bool isDead) { isDead_ = isDead; }
	/// <summary>
	/// 死亡状態ゲッター
	/// </summary>
	/// <returns>死亡状態</returns>
	bool GetIsDead() { return isDead_; }

	/// <summary>
	/// 打ち返し回数のゲッター
	/// </summary>
	/// <returns>打ち返し回数</returns>
	int32_t GetRallyCount() { return rallyCount_; }

	/// <summary>
	/// 色ゲッター
	/// </summary>
	/// <returns>色</returns>
	Vector4 GetColor() { return color_; }

	/// <summary>
	/// 場外判定セッター
	/// </summary>
	/// <param name="isFieldOut">場外判定</param>
	void SetIsFieldOut(const bool isFieldOut) { isFieldOut_ = isFieldOut; }
	/// <summary>
	/// 場外判定ゲッター
	/// </summary>
	/// <returns>場外判定</returns>
	bool GetIsFieldOut() { return isFieldOut_; }

	/// <summary>
	/// 麻痺パーティクルゲッター
	/// </summary>
	/// <returns>麻痺パーティクル</returns>
	Particle* GetStunParticle() { return stunParticle_; }
	/// <summary>
	/// 麻痺パーティクル停止関数
	/// </summary>
	void ResetStunParticle();

private: // メンバ変数

	// ゲームマネージャー
	GameManager* gameManager_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// 効果音群
	uint32_t damageSound_ = 0u; // ダメージ音

	// フィールドの範囲外に出ていないか
	bool isFieldOut_ = false;

	// アニメーションマネージャ
	AnimationManager* animManager_ = nullptr;
	// 敵アニメーション
	MyAnimation* enemyAnim_{};

	// 行動
	std::unique_ptr<IEnemyState> state_;
	// 行動変更クールタイム
	KLib::DeltaTimer stateChangeTimer_{};
	// 行動変更クールタイム定数値
	const float kStateChangeCoolTime_ = 1.5f;

	// 死亡トリガー
	bool isDead_ = false;
	// HP
	int32_t hp_ = 25;
	// ヒットクールタイムタイマー
	KLib::DeltaTimer hitCoolTimeTimer_{};
	// ヒットクールタイム定数値
	const float kHitCoolTime_ = 0.35f;

	// 身体のトランスフォーム
	WorldTransform bodyTransform_{};
	// 頭のトランスフォーム
	WorldTransform headTransform_{};
	// 右腕のトランスフォーム
	WorldTransform armTransform_R_{};
	// 左腕のトランスフォーム
	WorldTransform armTransform_L_{};

	// 当たり判定ワールド座標
	Vector3 worldPos_{};
	// 当たり判定半径
	float colliderRadius_ = 1.0f;

	// プレイヤー座標
	const WorldTransform* playerPos_ = nullptr;
	// プレイヤーへの角度
	float targetAngle_ = 0.0f;

	// ラリー回数記録用
	int32_t rallyCount_ = 0;
	// ラリーを行う最大回数
	const int32_t kMaxRallyCount_ = 5;

	// プレイヤーとの距離
	Vector3 toPlayerDistance_ = {};

	// 敵パーティクル
	Particle* enemyParticle_ = nullptr;
	// スタンパーティクル
	Particle* stunParticle_ = nullptr;

	/// デバック用変数群
	// 攻撃を行うか
	bool isAttack_ = true;
};

