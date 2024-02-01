#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Timer/DeltaTimer.h"
enum TutrialStep {
	tutrial1,
	tutrial2,
	tutrial3, 
	tutrial4,
	tutrial5,
	end,
};
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
     int step;
	 KLib::DeltaTimer timer_;
};

