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
	//下からの攻撃準備時間
	float pushUpPreAttackTime_= 1.0f;
	//下からの攻撃時間
	float pushUpAtackTime_= 0.5f;
	//下からの攻撃持続時間
	float pushUpPostAttackTime_= 4.0f;
	//ローラーの速さ
	float RollerSpeed_=0.2f;
	//	かけらの飛ぶ距離の最大
	float rubbleRange = 10.0f;
	//かけらの移動にかかる時間
	float rubblMoveTime_ = 1.0f;
private:
	void SetGlobalVariable();
	
};

