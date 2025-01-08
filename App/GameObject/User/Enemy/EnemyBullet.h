#pragma once
#include "Engine/GameObject/IObject.h"

/// <summary>
/// 敵の弾クラス
/// </summary>
class EnemyBullet : public IObject
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

public: // 機能関数群

	/// <summary>
	/// 落ち影の更新関数
	/// </summary>
	void ShadowUpdate();

	/// <summary>
	/// 弾の削除時に呼び出す関数
	/// </summary>
	void DeleteBullet();

	/// <summary>
	/// 軌跡パーティクルの再生関数
	/// </summary>
	void PlayTrailParticle();

	/// <summary>
	/// 命中パーティクル再生関数
	/// </summary>
	void PlayHitParticle();

	/// <summary>
	/// ヒットストップの開始関数
	/// </summary>
	/// <param name="hitStopTime">ヒットストップする秒数</param>
	/// <param name="timeScale">再生速度</param>
	void StartHitStop(const float hitStopTime, const float timeScale = 0.0f);

	/// <summary>
	/// ヒットストップの更新関数
	/// </summary>
	void HitStopUpdate();

public: // アクセッサ等

	/// <summary>
	/// プレイヤー座標のセッター
	/// </summary>
	/// <param name="transform">プレイヤー座標</param>
	void SetPlayerTransform(const WorldTransform* transform) { player_ = transform; }

	/// <summary>
	/// 敵座標のセッター
	/// </summary>
	/// <param name="transform">敵座標</param>
	void SetEnemyTransform(const WorldTransform* transform) { enemy_ = transform; }

	/// <summary>
	/// 速度のセッター
	/// </summary>
	/// <param name="isPlayer">プレイヤーに向けたベクトルか</param>
	/// <param name="rallyCount">打ち返し回数</param>
	void SetVelocity(const bool isPlayer, const int32_t rallyCount);

	/// <summary>
	/// 跳ね返し状態のセッター
	/// </summary>
	/// <param name="isReturn">跳ね返したか</param>
	void SetIsReturn(const bool isReturn) { isReturn_ = isReturn; }
	/// <summary>
	/// 跳ね返し状態のゲッター
	/// </summary>
	/// <returns>跳ね返しているのか</returns>
	bool GetIsReturn() { return isReturn_; }

	/// <summary>
	/// 弾と特定のボーンを親子付けする関数
	/// </summary>
	/// <param name="mat">親子付けするボーン行列</param>
	/// <param name="wt">親子付けするワールドトランスフォーム</param>
	void ParentPosition(Matrix4x4* mat, WorldTransform* wt);
	/// <summary>
	/// 親子付けの解除関数
	/// </summary>
	void UnParent();

private: // メンバ変数

	// 発射ベクトル
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	// 発射スピード
	float speed_ = 0.25f;
	// 最大発射スピード
	const float maxSpeed_ = 0.5f;
	// 加速度
	const float kAcceleration_ = 0.035f;

	// 速度
	float timeScale_ = 1.0f;

	// 落ち影のワールドトランスフォーム
	WorldTransform shadowTransform_{};
	// 落ち影のスケール補間値
	float maxShadowScale = 0.85f;
	float minShadowScale = 0.5f;
	// 敵のY座標閾値
	float maxHeight_ = 15.0f;

	// プレイヤーへ発射されているか
	bool isPlayer_ = false;

	// 一度跳ね返しているか
	bool isReturn_ = false;

	// ヒットストップ状態か
	bool isHitStop_ = false;
	// ヒットストップ用タイマー
	KLib::DeltaTimer hitStopTimer_{};
	// ヒットストップ倍率
	float hitStopTimeAcceleration_ = 3.0f;

	// プレイヤーの座標
	const WorldTransform* player_ = nullptr;
	// 敵の座標
	const WorldTransform* enemy_ = nullptr;

	// 指定座標との親子付け用行列
	Matrix4x4* parentMat_ = nullptr;

	// Dissolveの演出用フラグ
	bool isDissolving_ = false;
	bool isSwitchDissolving_ = false;

	// 当たり判定用ワールド座標
	Vector3 colliderPos_{};

	// 雷パーティクル
	Particle* sparkParticle_ = nullptr;
	// 弾パーティクル
	Particle* bulletParticle_ = nullptr;
	// 弾の軌跡パーティクル
	Particle* trailParticle_ = nullptr;

	// 効果音群
	uint32_t counterSound_ = 0u;
};

