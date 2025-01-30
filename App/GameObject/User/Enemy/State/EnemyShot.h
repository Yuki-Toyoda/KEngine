#pragma once
#include "IEnemyState.h"
#include "Engine/Utility/KLib.h"

// クラスの前方宣言
class EnemyBullet;

/// <summary>
/// 敵が弾を発射する処理
/// </summary>
class EnemyShot : public IEnemyState
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

	// 発射する弾
	EnemyBullet* bullet_ = nullptr;

	// 左腕行列
	Matrix4x4 leftArmMatrix_{};

	// 発射するまでのタイマー
	KLib::DeltaTimer timer_;
	// 発射するまでの時間
	const float kMinShotTime_ = 1.5f; // 最小
	const float kMaxShotTime_ = 2.5f; // 最大

	// チャージアニメーションへの遷移秒数
	const float chargeAnimTransitionTime_ = 0.1f;
	// 発射アニメーションへの遷移秒数
	const float shotAnimTransitionTime_ = 0.1f;

};

