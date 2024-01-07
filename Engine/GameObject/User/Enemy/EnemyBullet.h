#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 敵の弾クラス
/// </summary>
class EnemyBullet : public BaseObject
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
	void SetVelocity(const bool& isPlayer);

	/// <summary>
	/// 跳ね返し状態のセッター
	/// </summary>
	/// <param name="isReturn">跳ね返したか</param>
	void SetIsReturn(bool isReturn) { isReturn_ = isReturn; }

private: // メンバ変数

	// 発射ベクトル
	Vector3 velocity_ = { 0.0f, 0.0f, 0.0f };
	// 発射スピード
	float speed_ = 0.25f;
	// 最大発射スピード
	const float maxSpeed_ = 3.0f;

	// プレイヤーへ発射されているか
	bool isPlayer_ = false;

	// 一度跳ね返しているか
	bool isReturn_ = false;

	// プレイヤーの座標
	const WorldTransform* player_ = nullptr;
	// 敵の座標
	const WorldTransform* enemy_ = nullptr;
};

