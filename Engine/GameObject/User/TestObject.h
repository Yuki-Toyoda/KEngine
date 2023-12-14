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
};

