#pragma once
#include "../../BaseObject.h"
class TutrialManager:public BaseObject
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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;
private:
};

