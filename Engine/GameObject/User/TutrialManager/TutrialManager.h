#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Timer/DeltaTimer.h"
#include "../Enemy/Boss.h"
#include "../Player/Player.h"
#include "../Player/uribo/uribo.h"
#include "../camera/InGameCamera.h"
#include "../camera/TutrialCmera.h"

// クラスの前方宣言
class InGameUIManager;

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

	/// <summary>
	/// インゲームカメラのセッター
	/// </summary>
	/// <param name="camera">インゲームカメラ</param>
	void SetIngameCamera(InGameCamera* camera);

	/// <summary>
	/// インゲームUIマネージャーのセッター
	/// </summary>
	/// <param name="iGUIM">インゲームUIマネージャー本体</param>
	void SetInGameUIManger(InGameUIManager* iGUIM) { inGameUIManager_ = iGUIM; }

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

	 // インゲーム内のUIマネージャー
	 InGameUIManager* inGameUIManager_ = nullptr;

};

