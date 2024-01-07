#pragma once
#include "../../BaseObject.h"

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

public: // パブリックなメンバ変数

	

private: // メンバ変数

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;
	// 頭のトランスフォーム
	WorldTransform headTransform_;
	// 右腕のトランスフォーム
	WorldTransform armTransform_R_;
	// 左腕のトランスフォーム
	WorldTransform armTransform_L_;

	// 当たり判定のトランスフォーム
	WorldTransform colliderTransform_;

	float colliderRadius_ = 1.0f;

	Vector3 worldPos_;
};

