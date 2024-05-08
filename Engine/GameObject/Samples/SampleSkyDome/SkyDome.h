#pragma once
#include "../../IObject.h"

/// <summary>
/// 天球サンプル
/// </summary>
class SkyDome : public IObject
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

private: // メンバ変数
};

