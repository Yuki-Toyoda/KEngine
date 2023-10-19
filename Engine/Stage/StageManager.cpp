#include "StageManager.h"
#include "user/StageList.h"
#include "../GameObject/GameObjectManager.h"

using StageInfo = BaseStage::StageInfo;
using ItemInfo = BaseStage::ItemInfo;
using GearInfo = BaseStage::GearInfo;

StageManager* StageManager::GetInstance()
{
	static StageManager sceneManager;
	return &sceneManager;
}

void StageManager::Initialize()
{
	globalVariables_ = GlobalVariables::GetInstance();

	// ステージ初期化
	currentStage_ = nullptr;
	
	commitIndex = 0;
	loadIndex = 0;

	items_.clear();
	items_.resize(kMaxItem_);
	for (size_t i = 0; i < items_.size(); i++)
	{
		items_[i] = new Item();
		items_[i]->Initialize("Item", BaseObject::tagItem);
		items_[i]->SetIsActive(false);
		GameObjectManager::GetInstance()->AddGameObject(items_[i]);
	}

	AddlyGloavalVariables();
	// ステージの設定を読み込む
	LoadStages();
}

void StageManager::Update()
{
#ifdef _DEBUG

	DebugGUI();

#endif // _DEBUG

	// 現在のシーンの更新
	currentStage_->Update();

}

void StageManager::SetStage(size_t stageIndex)
{
#ifdef _DEBUG

	if (1) {
		StageInfo info{};
		info.gearInfo_.clearCondition_ = 100.0f;
		info.itemInfo_.clear();
		currentStage_ = new DebugStage();
		currentStage_->commonInitialize();
		currentStage_->Initialize(info);
		return;
	}

#endif // _DEBUG

	currentStage_ = new DebugStage();
	currentStage_->commonInitialize();
	currentStage_->Initialize(infos_[stageIndex]);

}

void StageManager::AddStageInfo(const BaseStage::StageInfo& info) {
	infos_.push_back(info);
	kMaxStageNum_ = static_cast<int32_t>(infos_.size());
}

void StageManager::LoadStages()
{
	infos_.clear();
	// 作ったステージの個数を取得
	kMaxStageNum_ = globalVariables_->GetIntValue("Stage", "kMaxStageNum");
	// ステージの回数分読み込む
	infos_.resize(kMaxStageNum_);
	for (size_t i = 0; i < infos_.size(); i++)
	{
		infos_.push_back(LoadInfo(i));
	}
}

void StageManager::AddlyGloavalVariables()
{
	// グループを作っておく
	globalVariables_->CreateGroup("Stage");
	globalVariables_->CreateGroup("StageInfo");

	globalVariables_->AddItem("Stage", "kMaxStageNum", kMaxStageNum_);
}

StageInfo StageManager::LoadInfo(size_t num)
{
	// Item の位置、リポップフラグ
	// クリアゲージ
	// 敵
	// カタパルトの個数、位置
	StageInfo info{};
	std::string indexNum = std::to_string(num) + ":";
	std::string strStage = "StageInfo";
	info.gearInfo_.clearCondition_ = globalVariables_->GetFloatValue(strStage, indexNum + "Condition");
	// アイテムの個数を取得
	int kMaxInfoNum = globalVariables_->GetIntValue(strStage, indexNum + "kMaxInfoNum");
	// アイテムを読み込む
	std::vector<ItemInfo>& iInfo = info.itemInfo_;
	iInfo.resize(kMaxInfoNum);
	for (size_t i = 0; i < iInfo.size(); i++) {
		iInfo[i].isRePop_ = static_cast<bool>(globalVariables_->GetIntValue(strStage, indexNum + "isRePop:" + std::to_string(i)));
		iInfo[i].position_ = globalVariables_->GetVector3Value(strStage, indexNum + "ItemPosition:" + std::to_string(i));
	}
	return info;
}

void StageManager::SaveStages()
{
	kMaxStageNum_ = static_cast<int32_t>(infos_.size());
	globalVariables_->AddItem("Stage", "kMaxStageNum", kMaxStageNum_);
	globalVariables_->SetValue("Stage", "kMaxStageNum", kMaxStageNum_);
	// ステージの数だけ書き出す
	for (size_t i = 0; i < infos_.size(); i++)
	{
		SaveInfo(i);
	}
}

void StageManager::SaveInfo(size_t num)
{
	std::string indexNum = std::to_string(num) + ":";
	std::string strStage = "StageInfo";
	StageInfo& info = infos_[num];
	info = currentStage_->GetStageInfo();
	globalVariables_->AddItem(strStage, indexNum + "Condition", info.gearInfo_.clearCondition_);
	globalVariables_->SetValue(strStage, indexNum + "Condition", info.gearInfo_.clearCondition_);
	int32_t kMaxInfoNum = static_cast<int32_t>(info.itemInfo_.size());
	globalVariables_->AddItem(strStage, indexNum + "kMaxInfoNum", kMaxInfoNum);
	globalVariables_->SetValue(strStage, indexNum + "kMaxInfoNum", kMaxInfoNum);
	std::vector<ItemInfo>& iInfo = info.itemInfo_;
	for (size_t i = 0; i < kMaxInfoNum; i++)
	{
		globalVariables_->AddItem(strStage, indexNum + "isRePop:" + std::to_string(i), static_cast<int32_t>(iInfo[i].isRePop_));
		globalVariables_->SetValue(strStage, indexNum + "isRePop:" + std::to_string(i), static_cast<int32_t>(iInfo[i].isRePop_));
		globalVariables_->AddItem(strStage, indexNum + "position:" + std::to_string(i), iInfo[i].position_);
		globalVariables_->SetValue(strStage, indexNum + "position:" + std::to_string(i), iInfo[i].position_);
	}
}

void StageManager::DebugGUI()
{
#ifdef _DEBUG

	ImGui::Begin("StageManager");

	ImGui::Text("StageNum:%d", infos_.size());
	ImGui::Text("kStageMaxNum:%d", kMaxStageNum_);
	ImGui::Separator();

	if (ImGui::Button("CommitAll")) {
		SaveStages();
	}
	if (ImGui::Button("CommitAs")) {
		kMaxStageNum_ = static_cast<int32_t>(infos_.size());
		SaveInfo(commitIndex);
	}
	ImGui::SameLine();
	ImGui::InputInt("index", &commitIndex, 1, 5);
	if (commitIndex < 0) {
		commitIndex = 0;
	}
	else if (infos_.size() - 1 < commitIndex) {
		commitIndex = static_cast<int>(infos_.size()) - 1;
	}
	ImGui::Separator();
	ImGui::Separator();
	if (ImGui::Button("LoadAll")) {
		LoadStages();
	}
	if (ImGui::Button("LoadAs")) {
		infos_.push_back(LoadInfo(loadIndex));
	}
	ImGui::SameLine();
	ImGui::InputInt("index", &loadIndex, 1, 5);
	if (loadIndex < 0) {
		loadIndex = 0;
	}
	ImGui::Separator();

	if (ImGui::Button("PopBack")) {
		if (infos_.size() != 0) {
			infos_.pop_back();
		}
	}

	ImGui::End();

#endif // _DEBUG
}