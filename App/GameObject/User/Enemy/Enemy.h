#pragma once
#include "Engine/GameObject/IObject.h"
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
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	void OnCollisionEnter(Collider* target, [[maybe_unused]] Collider* source) override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	void OnCollision(Collider* target, [[maybe_unused]] Collider* source) override;

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	void OnCollisionExit(Collider* target, [[maybe_unused]] Collider* source) override;

	/// <summary>
	/// 行動状態変更関数
	/// </summary>
	/// <param name="newState"></param>
	void ChangeState(std::unique_ptr<IEnemyState> newState);

	/// <summary>
	/// ヒットストップの開始関数
	/// </summary>
	/// <param name="hitStopTime">ヒットストップする秒数</param>
	/// <param name="timeScale">再生速度</param>
	void StartHitStop(const float hitStopTime, float timeScale = 0.0f);

	/// <summary>
	/// ヒットストップの更新関数
	/// </summary>
	void HitStopUpdate();

	/// <summary>
	/// 落ち影更新関数
	/// </summary>
	void ShadowUpdate();

	/// <summary>
	/// 死亡演出時のカメラのセットアップ関数
	/// </summary>
	void SetUpDeadCameraStaging();

	/// <summary>
	/// 死亡時のカメラ演出関数
	/// </summary>
	void DeadCameraStaging();

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
	/// 現在行動の名前ゲッター
	/// </summary>
	/// <returns>現在行動の名前</returns>
	std::string GetStateName() { return state_->GetStateName(); }

	/// <summary>
	/// 左腕のワールド座標ゲッター
	/// </summary>
	/// <returns>左腕のワールド座標</returns>
	Vector3 GetLeftArmPosition() { return transform_.GetWorldPos() + skiningModels_["Enemy"]->GetBonePosition("Hand.L"); }

	/// <summary>
	/// 回転固定状態のセッター
	/// </summary>
	/// <param name="isLock">回転固定</param>
	void SetIsRotateLock(const bool isLock) { isRotateLock_ = isLock; }

	/// <summary>
	/// 死亡演出トリガーのゲッター
	/// </summary>
	/// <returns>死亡演出状態</returns>
	bool GetIsDeadStaging() { return isDeadStaging_; }

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
	/// 攻撃可能状態セッター
	/// </summary>
	/// <param name="canAttack">攻撃可能状態</param>
	void SetCanAttack(const bool canAttack) { canAttack_ = canAttack; }

	/// <summary>
	/// 近接攻撃状態セッター
	/// </summary>
	/// <param name="isAttack">攻撃状態</param>
	void SetIsCQCAttack(const bool isAttack) { isCQCAttack_ = isAttack; }
	/// <summary>
	/// 近接攻撃状態ゲッター
	/// </summary>
	/// <returns>近接攻撃状態</returns>
	bool GetIsCQCAttack() { return isCQCAttack_; }

	/// <summary>
	/// 麻痺パーティクルの再生関数
	/// </summary>
	void PlayStunParticle();
	/// <summary>
	/// 麻痺パーティクルゲッター
	/// </summary>
	/// <returns>麻痺パーティクル</returns>
	Particle* GetStunParticle() { return stunParticle_; }

	/// <summary>
	/// 敵パーティクルゲッター
	/// </summary>
	/// <returns>敵パーティクル</returns>
	Particle* GetEnemyParticle() { return enemyParticle_; }

	/// <summary>
	/// 麻痺パーティクル停止関数
	/// </summary>
	void ResetStunParticle();

	/// <summary>
	/// ダメージパーティクルの再生関数
	/// </summary>
	void PlayDamageParticle();

	/// <summary>
	/// ダウン状態から起き上がらない状態のゲッター
	/// </summary>
	/// <returns>ダウン状態から起き上がらないか</returns>
	bool GetIsNeverDown() { return isNeverDown_; }

	/// <summary>
	/// ロックオン可能状態のゲッター
	/// </summary>
	/// <returns>ロックオン可能状態</returns>
	bool GetCanLockOn() const { return canLockOn_; }
	/// <summary>
	/// ロックオン可能状態のセッター
	/// </summary>
	/// <param name="canLockOn">ロックオン可能状態</param>
	void SetCanLockOn(const bool canLockOn) { canLockOn_ = canLockOn; }

	/// <summary>
	/// コライダーワールド座標のゲッター
	/// </summary>
	/// <returns>コライダーワールド座標</returns>
	Vector3 GetColliderPos() { return worldPos_; }

	/// <summary>
	/// コライダー半径のゲッター
	/// </summary>
	/// <returns>コライダー半径</returns>
	float GetColliderRadius() { return colliderRadius_; }

	/// <summary>
	/// 身体のワールドトランスフォームゲッター
	/// </summary>
	/// <returns>身体のワールドトランスフォーム</returns>
	WorldTransform* GetBodyTransform() { return &bodyTransform_; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 行動変更に関する更新関数
	/// </summary>
	void ChangeStateUpdate();

private: // メンバ変数

	// ゲームマネージャー
	GameManager* gameManager_ = nullptr;

	// プレイヤー
	Player* player_ = nullptr;

	// 効果音群
	uint32_t damageSound_ = 0u; // ダメージ音

	// フィールドの範囲外に出ていないか
	bool isFieldOut_ = false;

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

	// ヒットストップ状態か
	bool isHitStop_ = false;
	// ヒットストップ用タイマー
	KLib::DeltaTimer hitStopTimer_{};
	// とどめを指した場合のヒットストップ秒数
	float finishHitStopTime_ = 5.0f;
	// 死亡時のアニメーション速度
	float deadStagingAnimSpeed_ = 0.1f;
	// とどめ時のブラーの演出時間
	const float finishBlurStagingTime_ = 0.01f;
	const float finishBlurEndTime_ = 1.5f;
	const float finishBlurStrength_ = 0.065f;

	// 死亡時のカメラ演出中かのフラグ
	bool isDeadStaging_ = false;
	// 死亡時の演出カメラ
	std::vector<Camera*> deadStagingCameras_{};
	// カメラ数
	const int32_t kDeadStagingCameraCount_ = 1;
	// 表示中カメラ番号
	int32_t displayCameraNumber_ = 0;
	// カメラ切り替え用タイマー
	KLib::DeltaTimer switchCameraTimer_{};
	// カメラ切り替え秒数
	float kCameraSwitchTime_ = 0.0f;

	// 死亡演出中のFOV加算値
	const float deadstagingAddFOV_ = 0.00025f;

	// 身体のトランスフォーム
	WorldTransform bodyTransform_{};
	// 武器のトランスフォーム
	WorldTransform weaponTransform_{};

	// 当たり判定ワールド座標
	Vector3 worldPos_{};
	// 当たり判定半径
	float colliderRadius_ = 1.0f;

	// 落ち影のワールドトランスフォーム
	WorldTransform shadowTransform_{};
	// 落ち影のスケール補間値
	float maxShadowScale = 2.5f;
	float minShadowScale = 1.75f;
	// 敵のY座標閾値
	float maxHeight_ = 3.0f;
	// 影高さ
	const float shadowHeight_ = 0.05f;

	// プレイヤー座標
	const WorldTransform* playerPos_ = nullptr;
	// プレイヤーへの角度
	float targetAngle_ = 0.0f;
	// 目標角度への補正スピード
	const float angleCorrectSpeed = 0.1f;
	// 敵をプレイヤーの方に向かせるか
	bool isRotateLock_ = false;

	// 攻撃可能状態か
	bool canAttack_ = false;

	// ラリー回数記録用
	int32_t rallyCount_ = 0;
	// ラリーを行う最大回数
	const int32_t kMaxRallyCount_ = 6;

	// プレイヤーとの距離
	Vector3 toPlayerDistance_ = {};
	// 移動行動に移る際の最小距離
	const float minPlayerDistance_ = 10.0f;
	// 移動行動の繰り返しカウント
	int32_t moveCount_ = 0;
	const int32_t maxMoveCount_ = 2;

	// 近接攻撃中か
	bool isCQCAttack_ = false;
	// 近接攻撃時の判定座標
	Vector3 attackColliderPosition_{};
	// 近接攻撃時の判定大きさ
	float attackColliderRadius_ = 1.5f;
	// 近接攻撃当たり判定のオフセット
	const Vector3 kAttackColliderOffset = { 0.0f, 0.0f, -3.0f };

	// ロックオン可能状態
	bool canLockOn_ = true;

	// 敵パーティクル
	Particle* enemyParticle_ = nullptr;
	// スタンパーティクル
	Particle* stunParticle_ = nullptr;

	/// デバック用変数群
	// 攻撃を行うか
	bool isAttack_ = true;
	// 一度ダウンしたら起き上がらない
	bool isNeverDown_ = false;
	// 敵のHP無限
	bool isInfiniteHP_ = false;
};

