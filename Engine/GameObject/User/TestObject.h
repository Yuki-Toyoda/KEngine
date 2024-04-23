#pragma once
#include "../BaseObject.h"
#include "../../Utility/Animation/AnimationManager.h"
#include "../../Utility/Line/Line.h"
#include "../../Audio/Audio.h"

/// <summary>
/// テストオブジェクト
/// </summary>
class TestObject : public BaseObject
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
	/// <param name="collider">衝突したコライダー</param>
	void OnCollisionEnter(Collider* collider) override;

	/// <summary>
	/// 衝突していたオブジェクトから離れた時のコールバック関数
	/// </summary>
	/// <param name="collider">衝突していたコライダー<</param>
	void OnCollisionExit(Collider* collider) override;

private: // プライベートなメンバ変数

	// テスト用変数達
	float testFloatValue_ = 0.0f;
	Vector2 testVector2Value_ = {0.0f, 0.0f};
	Vector3 testVector3Value_ = { 0.0f, 0.0f, 0.0f };
	float testTransitionTime_ = 0.5f;

	// 線だ！！！
	std::unique_ptr<Line> line_;

	KLib::DeltaTimer timer_;

	uint32_t sound_ = 0u;

	// アニメーションの管理用
	MyAnimation* animation_ = nullptr;
};

