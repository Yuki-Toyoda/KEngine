#pragma once
#include "../../../BaseObject.h"
#include "../../Rubble/Rubble.h"
#include "../../GameManager/GameManager.h"
#include "../../../../Particle/ParticleEmitterManager.h"

class Meteor:public BaseObject
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
	/// ゲームマネージャーセッター
	/// </summary>
	/// <param name="gamemanager"></param>
	void SetgameManager(GameManager* gamemanager) { gameManager_ = gamemanager; }

public: // アクセッサ等

	/// <summary>
	/// 落下トリガーセッター
	/// </summary>
	/// <param name="isFall">落下トリガー状態</param>
	void SetIsFall(const bool& isFall) { isFall_ = isFall; }

private: // プライベートなメンバ関数

	/// <summary>
	/// 衝突時のみコールバックされる関数
	/// </summary>
	/// <param name="collider">当たったコライダー</param>
	void OnCollisionEnter(Collider* collider)override;

private: // メンバ変数

	// 移動方向ベクトル
	Vector3 velocity_;
	// 移動スピード
	float moveSpeed_ = 0.1f;
	// 当たり判定用ワールド座標
	Vector3 worldPos_;
	
	// 攻撃範囲開示用トランスフォーム
	WorldTransform attackAreaTransform_;
	// 攻撃範囲色
	Vector4 areaColor_;

	// ゲームマネージャー
	GameManager* gameManager_;
	
	// 落下トリガー
	bool isFall_ = false;

	// パーティクルエミッタマネージャ
	ParticleEmitterManager* pem_ = nullptr;

};

