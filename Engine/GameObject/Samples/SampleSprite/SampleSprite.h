#pragma once
#include "../../BaseObject.h"

/// <summary>
/// スプライトの描画を行うサンプル
/// </summary>
class SampleSprite : public BaseObject
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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

};

