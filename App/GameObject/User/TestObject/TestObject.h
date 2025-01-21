#pragma once
#include "Engine/GameObject/IObject.h"

// クラスの前方宣言
class MyAnimation;

/// <summary>
/// テストオブジェクト
/// </summary>
class TestObject : public IObject
{
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui() override;

public: // その他関数

	/// <summary>
	/// 衝突した瞬間にコールバックされる関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	void OnCollisionEnter(Collider* target, Collider* source) override;

	/// <summary>
	/// 衝突時コールバック関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	void OnCollision(Collider* target, Collider* source) override;

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="target">衝突した対象のコライダー</param>
	/// <param name="source">衝突した自分のコライダー</param>
	void OnCollisionExit(Collider* target, Collider* source) override;

private: // プライベートなメンバ変数

	// テスト用変数達
	float testFloatValue_ = 0.0f;
	Vector2 testVector2Value_ = {0.0f, 0.0f};
	Vector3 testVector3Value_ = { 0.0f, 0.0f, 0.0f };

	KLib::DeltaTimer timer_;

	// アニメーションの管理用
	MyAnimation* animation_ = nullptr;

	// パーティクルテスト用
	Particle* p = nullptr;
};

