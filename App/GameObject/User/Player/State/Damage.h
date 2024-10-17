#pragma once
#include "IState.h"
#include "Engine/Utility/KLib.h"

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

	// 吹っ飛ばされる速度ベクトル
	Vector3 velocity_{};
	// 演出用カメラ
	Camera* c = nullptr;
	// 演出用タイマー
	KLib::DeltaTimer timer_;

	// 吹っ飛ばし時間閾値
	const float blowTimeThreshold_ = 0.25f;
};

