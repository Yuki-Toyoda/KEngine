#include "DebugStage.h"
#include "../StageManager.h"
#include "../../GameObject/Item/Item.h"
#include "../../GameObject/Player/Player.h"
#include "../../GameObject/Catapult/Catapult.h"

void DebugStage::Initialize(const StageInfo& info)
{
	stageInfo_ = info;
	ApplyStageInfo();
}

void DebugStage::Update()
{
	float gearRotate{ 0 };
	gearRotate += player_->GetGearRotateSpeed();
	if(gearRotate >= player_->GetAddGearSpeedLimit() || gearRotate <= -player_->GetAddGearSpeedLimit())
		if(isAddGearCondition_)
			gearCondition_ += 0 < gearRotate ? gearRotate : gearRotate * -1;
	DebugGUI();
}

void DebugStage::DebugGUI()
{
#ifdef _DEBUG


	std::vector<Item*>& items = stageManager_->GetItems();
	std::vector<ItemInfo>& iInfo = stageInfo_.itemInfo_;
	std::vector<Catapult*>& catapults = stageManager_->GetCatapult();
	std::vector<CatapultInfo>& cInfo = stageInfo_.catapultInfo_;

	ImGui::Begin("DebugStage");

	ImGui::DragFloat("ClearCondition", &stageInfo_.gearInfo_.clearCondition_, 1.0f, 1.0f);

	ImGui::Separator();
	if (ImGui::Button("NewItem")) {
		// アイテムが最大数に達していないなら
		if (iInfo.size() < stageManager_->GetItemMax()) {
			ItemInfo info{};
			info.isRePop_ = true;
			info.position_ = { 0.0f,0.0f,0.0f };
			info.popTime_ = 10;
			items[iInfo.size()]->SetIsRePop(info.isRePop_);
			items[iInfo.size()]->SetPosition(info.position_);
			items[iInfo.size()]->SetIsActive(true);
			iInfo.push_back(info);
		}
	}
	ImGui::SameLine();
	// 一番後ろのアイテムを非表示にする
	if (ImGui::Button("DeleteItem")) {
		if (iInfo.size() != 0) {
			items[iInfo.size() - 1]->SetIsActive(false);
			iInfo.pop_back();
		}
	}
	ImGui::Separator();
	std::string commitIndex_ = "info";
	ImGui::Text("index:%d", iInfo.size());
	for (size_t i = 0; i < iInfo.size(); i++)
	{
		std::string treeName = commitIndex_ + std::to_string(i);
		if (ImGui::TreeNode(treeName.c_str())) {
			ImGui::DragFloat2("positon", &iInfo[i].position_.x, 0.01f);
			ImGui::Checkbox("isRePop", &iInfo[i].isRePop_);
			ImGui::SliderInt("popTIme", &iInfo[i].popTime_, 10, 1000);

			ImGui::TreePop();
		}
		ImGui::Separator();
	}

	ImGui::Separator();
	if (ImGui::Button("NewCatapult")) {
		// アイテムが最大数に達していないなら
		if (cInfo.size() < stageManager_->GetCatapultMax()) {
			CatapultInfo cinfo{};
			cinfo.theta_ = 0.0f;
			cinfo.length_ = 1.0f;
			cinfo.isRePop_ = true;
			cinfo.popTime_ = 10;
			catapults[cInfo.size()]->SetIsRePop(cinfo.isRePop_);
			catapults[cInfo.size()]->SetIsActive(true);
			cInfo.push_back(cinfo);
		}
	}
	ImGui::SameLine();
	// 一番後ろのアイテムを非表示にする
	if (ImGui::Button("DeleteCatapult")) {
		if (cInfo.size() != 0) {
			catapults[cInfo.size() - 1]->SetIsActive(false);
			cInfo.pop_back();
		}
	}
	ImGui::Separator();
	commitIndex_ = "cinfo";
	ImGui::Text("CatapultIndex:%d", cInfo.size());
	for (size_t i = 0; i < cInfo.size(); i++)
	{
		std::string treeName = commitIndex_ + std::to_string(i);
		if (ImGui::TreeNode(treeName.c_str())) {
			ImGui::SliderFloat("theta", &cInfo[i].theta_, -3.14f, 3.14f);
			ImGui::SliderFloat("length", &cInfo[i].length_, 0.0f, 14.5f);
			ImGui::Checkbox("isRePop", &cInfo[i].isRePop_);
			ImGui::SliderInt("popTIme", &cInfo[i].popTime_, 10, 1000);

			ImGui::TreePop();
		}
		ImGui::Separator();
	}


	//ImGui::Separator();
	//if (ImGui::Button("Apply")) {
	//	ApplyStageInfo();
	//}

	ImGui::Checkbox("isApplyInfor", &isApplyInfo_);
	if (isApplyInfo_) {
		ApplyStageInfo();
	}

	ImGui::Separator();
	if (ImGui::Button("AddInfo")) {
		stageManager_->AddStageInfo(stageInfo_);
	}

	ImGui::End();

#endif // _DEBUG
}
