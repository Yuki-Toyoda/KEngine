#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Animation/AnimationManager.h"
#include "State/EnemyStateList.h"

// クラスの前方宣言
class Player;

/// <summary>
/// 敵クラス
/// </summary>
class Enemy : public BaseObject
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
	/// パラメータを作成する
	/// </summary>
	/// <param name="name">作成するパラメータ</param>
	void CreateParameter(const std::string& name);

	/// <summary>
	/// 行動状態変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IEnemyState> newState);

	/// <summary>
	/// プレイヤー座標セッター
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	void SetPlayerPos(const WorldTransform* playerPos) { playerPos_ = playerPos; }

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
	/// ゲーム開始フラグセッター
	/// </summary>
	/// <param name="isStart">ゲームを開始しているか</param>
	void SetIsGameStart(bool isStart) { isGameStart_ = isStart; }

public: // パブリックなメンバ変数

	// 左腕のトランスフォーム
	WorldTransform armTransform_L_;

	// 敵アニメーション
	MyAnimation* enemyAnim_;

	// 死亡トリガー
	bool isDead_ = false;

	// プレイヤー
	Player* player_ = nullptr;

private: // メンバ変数

	// アニメーションマネージャ
	AnimationManager* animManager_;

	// 行動
	std::unique_ptr<IEnemyState> state_;
	// 行動変更クールタイム
	KLib::DeltaTimer stateChangeTimer_;
	// 行動変更クールタイム定数値
	const float kStateChangeCoolTime_ = 1.5f;

	// ゲームスタートフラグ
	bool isGameStart_ = false;

	// HP
	int32_t hp_ = 25;
	// ヒットクールタイムタイマー
	KLib::DeltaTimer hitCoolTimeTimer_;
	// ヒットクールタイム定数値
	const float kHitCoolTime_ = 0.35f;

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;
	// 頭のトランスフォーム
	WorldTransform headTransform_;
	// 右腕のトランスフォーム
	WorldTransform armTransform_R_;

	// 当たり判定ワールド座標
	Vector3 worldPos_;
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
};

