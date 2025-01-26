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
	Vector3 velocity_ = { 0.0f, 0.0f, -0.25f };
	// 演出用カメラ
	Camera* c = nullptr;
	// 演出用タイマー
	KLib::DeltaTimer timer_;

	// 吹っ飛ばし時間閾値
	const float blowTimeThreshold_ = 0.25f;

	// 画面彩度の目標値
	const float targetSaturation_ = -1.0f;
	// ガウシアンブラー強さの目標値
	const float targetBlurStrength_ = 3.0f;

	// 死亡演出時間
	const float deadStagingTime_ = 1.5f;

	// カメラ移動スピード
	const float cameraMoveSpeed_ = -0.0025f;
};

