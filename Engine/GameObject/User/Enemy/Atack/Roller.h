#pragma once
#include "../../../BaseObject.h"
#include "../../GameManager/GameManager.h"

/// <summary>
/// ローラー攻撃
/// </summary>
class Roller :public BaseObject {
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

private:
	void OnCollisionEnter(Collider* collider)override;
private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動スピード
	float moveAcceleration_ = 0.3f;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;

	// ゲームマネージャー
	GameManager* gameManager_;

	// 動作トリガー
	bool isMove_ = false;

public: // アクセッサ

	/// <summary>
	/// 速度ベクトルゲッター
	/// </summary>
	/// <returns>速度ベクトル</returns>
	Vector3 GetVelocity() { return velocity_; }
	/// <summary>
	/// 速度ベクトルセッター
	/// </summary>
	/// <param name="velocity">速度ベクトル</param>
	void SetVelocity(const Vector3& velocity);

	/// <summary>
	/// ゲームマネージャーセッター
	/// </summary>
	/// <param name="gamemanager">ゲームマネージャー</param>
	void SetgameManager(GameManager* gamemanager) { gameManager_ = gamemanager; }

	/// <summary>
	/// 動作トリガーセッター
	/// </summary>
	/// <param name="isMove">動作させるか</param>
	void SetIsMove(const bool& isMove) { isMove_ = isMove; }
};