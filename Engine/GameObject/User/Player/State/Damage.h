#pragma once
#include "IState.h"
#include "../../../../Utility/KLib.h"

// クラスの前方宣言
class Camera;

/// <summary>
/// ダメージを喰らった時の処理
/// </summary>
class Damage : public IState
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
	/// ImGuiの表示関数
	/// </summary>
	void DisplayImGui() override;

private: // メンバ変数

	Vector3 velocity_;

	bool isDead_ = false;

	Camera* c;

	KLib::DeltaTimer timer_;

	// 起き上がり中
	bool recovering_ = false;

};

