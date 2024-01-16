#pragma once
#include "../IEnemyState.h"

/// <summary>
/// サンプルの敵の行動クラス(Vector3で指定した座標から座標までを指定した秒数かけて移動する)
/// </summary>
class EnemySampleState : public IEnemyState
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	/// <returns></returns>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

private: // メンバ変数

	// 線形補間用タイマー
	KLib::DeltaTimer lerpTimer_;

	// 遷移にかかる秒数
	float lerpTime_ = 3.0f;

	// 始端座標
	Vector3 start_ = {0.0f, 0.0f, 0.0f};
	// 始端座標
	Vector3 end_ = { 0.0f, 0.0f, 0.0f };

};

