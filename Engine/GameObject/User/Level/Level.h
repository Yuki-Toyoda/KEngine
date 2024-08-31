#pragma once
#include "../../Samples/SampleLevel/SampleLevelObjects.h"
#include "../../../Level/LevelLoader.h"

/// <summary>
/// レベルクラス
/// </summary>
class Level : public SampleLevelObjects
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
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui() override;

private: // メンバ変数

	// コライダーの中心座標
	Vector3 colliderPosition_ = {};

	//　コライダーの半径
	float colliderRadius_ = 30.0f;
};

