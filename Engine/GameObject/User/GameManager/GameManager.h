#pragma once
#include "../../BaseObject.h"
class GameManager:public BaseObject
{
public:
	void Init()override;
	void Update()override;
	void DisplayImGui()override;

private:
	//かけらが生きている時間
	float rubbleAliveCount_=5.0f;
	//かけらのサイズ
	float rubbleSize_;
	//上からフルスピード
	float meteorSpeed_;
	//下からの開店時間
	float pushUpRotateSpeed_;
	//下からの攻撃時間
	float pushUpAtackTime_;
	//下からの攻撃の判定の残る時間
	float pushUpRemainTime_;
};

