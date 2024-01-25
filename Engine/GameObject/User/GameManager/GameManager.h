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
	//かけらの当たり判定
	Vector3 rubbleCollisionScale_;
	//下からの開店時間
	float pushUpRotateSpeed_=3.0f;
	//下からの攻撃時間
	float pushUpAtackTime_=3.0f;
	//下からの攻撃の判定の残る時間
	float pushUpRemainTime_=3.0f;
	//ローラーの速さ
	float RollerSpeed_=0.2f;
	//	かけらの飛ぶ距離の最大
	float rubbleRange = 10.0f;
	//かけらの移動にかかる時間
	float rubblMoveTime_ = 1.0f;
private:
	void SetGlobalVariable();
	
};

