#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Timer/DeltaTimer.h"
#include "../Enemy/Boss.h"
#include "../Player/Player.h"
#include "../Player/uribo/uribo.h"
enum TutrialStep {
	yokero,
	atumero,
	kaihuku,
	kougeki,
	end,
};
class TutrialManager:public BaseObject
{
public:
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
	
public://アクセッサ
	void SetBoss(Boss* boss) { boss_ = boss; };
	void SetPlayer(Player* player) { player_ = player; }
	void SetUribo(Uribo* uribo) { uribo_ = uribo; }
	bool GetTutrialEnd() { return isEnd_; }
private:
     int step;
	 KLib::DeltaTimer timer_;

	 Boss* boss_;
	 Player* player_;
	 Uribo* uribo_;
	 int skipCount_;
	 bool isEnd_;
};

