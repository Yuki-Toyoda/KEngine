#include "GameManager.h"

void GameManager::Init()
{
}

void GameManager::Update()
{
}

void GameManager::DisplayImGui()

{

	
	//上からフルスピード
	ImGui::DragFloat("meteorSpeed", &meteorSpeed_,0.1f);
	//下からの開店時間
	ImGui::DragFloat("PushUpRotateSpeed" ,&pushUpRotateSpeed_,0.1f);
	//下からの攻撃時間
	ImGui::DragFloat("pushUpAtackTime",&pushUpAtackTime_);
	
	ImGui::DragFloat("pushUpRemain", &pushUpRemainTime_);
}
