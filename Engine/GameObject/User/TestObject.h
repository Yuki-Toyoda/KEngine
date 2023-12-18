#pragma once
#include "../BaseObject.h"

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
};

