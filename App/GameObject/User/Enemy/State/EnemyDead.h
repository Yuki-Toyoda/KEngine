#pragma once
#include "IEnemyState.h"
#include "Engine/Utility/Timer/DeltaTimer.h"

// クラスの前方宣言
class Camera;

/// <summary>
/// 敵の死亡アニメーションの状態
/// </summary>
class EnemyDead : public IEnemyState
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

	// 死亡演出用カメラ
	Camera* camera_ = nullptr;
	// フェードアウト開始フラグ
	bool isFadeOut_ = false;

	// ディゾルブ演出の開始フラグ
	bool isStartDissolveStaging_ = false;
	// ディゾルブ時のエッジ色
	const Vector3 deadDissolveEdgeColor_ = { 0.0f, 1.0f, 0.75f };
	// ディゾルブを光らせる閾値
	const float deadDissolveEdgeThreshold_ = 0.025f;
	// ディゾルブ時間計測用タイマー
	KLib::DeltaTimer dissolveStagingTimer_{};
	// ディゾルブ演出定数
	const float dissolveStagingTime_ = 3.0f;
};

