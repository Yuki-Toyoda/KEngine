#pragma once
#include "../../BaseObject.h"
class GameManager:public BaseObject
{
public:

	void Init()override;
	void Update()override;
	void DisplayImGui()override;

public:
	//かけらが生きている時間
   float rubbleAliveCount_=5.0f;
	//かけらのサイズ
 	float rubbleSize_=1.0f;
	//上からフルスピード
	float meteorSpeed_=0.2f;
	//下からの開店時間
	float pushUpRotateSpeed_=3.0f;
	//下からの攻撃時間
	float pushUpAtackTime_=3.0f;
	//下からの攻撃の判定の残る時間
	float pushUpRemainTime_=3.0f;
	//ローラーの速さ
	float RollerSpeed_=0.2f;
private:
	void SetGlobalVariable();
	
};

