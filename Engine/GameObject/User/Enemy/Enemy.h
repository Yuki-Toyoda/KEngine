#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Animation/AnimationManager.h"
#include "State/EnemyStateList.h"

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
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="collider">衝突していたコライダー<</param>
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

	/// <summary>
	/// プレイヤー座標セッター
	/// </summary>
	/// <param name="playerPos">プレイヤー座標</param>
	void SetPlayerPos(const WorldTransform* playerPos) { playerPos_ = playerPos; }

public: // パブリックなメンバ変数

	// 左腕のトランスフォーム
	WorldTransform armTransform_L_;

	// 敵アニメーション
	Animation* enemyAnim_;

private: // メンバ変数

	// アニメーションマネージャ
	AnimationManager* animManager_;

	// 行動
	std::unique_ptr<IEnemyState> state_;

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
};
