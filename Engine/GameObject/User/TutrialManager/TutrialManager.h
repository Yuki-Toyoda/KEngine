#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Timer/DeltaTimer.h"
#include "../Enemy/Boss.h"
#include "../Player/Player.h"
#include "../Player/uribo/uribo.h"
#include "../camera/InGameCamera.h"
#include "../camera/TutrialCmera.h"
enum TutrialStep {
	yokero,
	atumero,
	cameraMove,
	onakasuita,
	cameraBack,
	kaihuku,
	Dash,
	kougeki,
	end,
	plactice,
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

	void SetTutrialCamera(TutrialCamera* camera) { tCamera_ = camera; }
	void SetIngameCamera(InGameCamera* camera) { iCamera_ = camera; }
private:
     int step;
	 KLib::DeltaTimer timer_;

	 Boss* boss_;
	 Player* player_;
	 Uribo* uribo_;
	 int skipCount_;
	 bool isEnd_;
	 InGameCamera* iCamera_;
	 TutrialCamera* tCamera_;
};

