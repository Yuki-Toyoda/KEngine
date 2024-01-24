#include "GameManager.h"

#include"../../../GlobalVariables/GlobalVariables.h"


void GameManager::Init()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();
	variables->CreateGroup(name_);
	//上からフルスピード
	variables->AddItem(name_,"meteorSpeed", meteorSpeed_);
	//下からの開店時間
	variables->AddItem(name_, "PushUpRotateSpeed", pushUpRotateSpeed_);
	//下からの攻撃時間
	variables->AddItem(name_, "pushUpAtackTime", pushUpAtackTime_);

	variables->AddItem(name_, "pushUpRemain", pushUpRemainTime_);

	variables->AddItem(name_,"RubbleAlliveCount", rubbleAliveCount_);
	variables->AddItem(name_,"RubbleSize", rubbleSize_);
	variables->AddItem(name_, "rollerSpeed", RollerSpeed_);
	SetGlobalVariable();
}

void GameManager::Update()
{
	
}

void GameManager::DisplayImGui()

{
	ImGui::DragFloat("RollerSpeed", &RollerSpeed_, 0.1f);
	ImGui::DragFloat("RubbleAlliveCount", &rubbleAliveCount_, 0.1f);
	ImGui::DragFloat("RubbleSize", &rubbleSize_, 0.1f);
	//上からフルスピード
	ImGui::DragFloat("meteorSpeed", &meteorSpeed_, 0.1f);
	//下からの開店時間
	ImGui::DragFloat("PushUpRotateTime", &pushUpRotateSpeed_, 0.1f);
	//下からの攻撃時間
	ImGui::DragFloat("pushUpAtackTime", &pushUpAtackTime_);

	ImGui::DragFloat("pushUpRemainTime", &pushUpRemainTime_);
	if (ImGui::Button("save")) {
		GlobalVariables* variables = GlobalVariables::GetInstance();

		//上からフルスピード
		variables->SetValue(name_, "meteorSpeed", meteorSpeed_);
		//下からの開店時間
		variables->SetValue(name_, "PushUpRotateSpeed", pushUpRotateSpeed_);
		//下からの攻撃時間
		variables->SetValue(name_, "pushUpAtackTime", pushUpAtackTime_);

		variables->SetValue(name_, "pushUpRemain", pushUpRemainTime_);

		variables->SetValue(name_, "RubbleAlliveCount", rubbleAliveCount_);
		variables->SetValue(name_, "RubbleSize", rubbleSize_);
		variables->SetValue(name_, "rollerSpeed", RollerSpeed_);
		variables->SaveFile(name_);
		SetGlobalVariable();
	}
}

void GameManager::SetGlobalVariable()
{
	GlobalVariables* variables = GlobalVariables::GetInstance();

	//上からフルスピード
	meteorSpeed_=variables->GetFloatValue(name_, "meteorSpeed");
	//下からの開店時間
	pushUpRotateSpeed_= variables->GetFloatValue(name_, "PushUpRotateSpeed");
	//下からの攻撃時間
	pushUpAtackTime_ = variables->GetFloatValue(name_, "pushUpAtackTime");

	pushUpRemainTime_=variables->GetFloatValue(name_, "pushUpRemain");

	rubbleAliveCount_=variables->GetFloatValue(name_, "RubbleAlliveCount" );
	rubbleSize_=variables->GetFloatValue(name_, "RubbleSize");
	RollerSpeed_=variables->GetFloatValue(name_, "rollerSpeed");
}
