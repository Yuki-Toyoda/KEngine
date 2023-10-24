#include "StageManager.h"
#include "user/StageList.h"
#include "../GameObject/GameObjectManager.h"
#include "../GameObject/Item/Item.h"
#include "../GameObject/Catapult/Catapult.h"

using StageInfo = BaseStage::StageInfo;
using ItemInfo = BaseStage::ItemInfo;
using GearInfo = BaseStage::GearInfo;
using CatapultInfo = BaseStage::CatapultInfo;

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

#ifdef _DEBUG

	commitIndex_ = 0;
	loadIndex_ = 0;

#endif // DEBUG

	nowStageNum_ = 0;
	gameTime_ = 0;

	GameObjectManager* manager = GameObjectManager::GetInstance();

	for (Item* item : items_) {
		delete item;
	}
	items_.clear();
	items_.resize(kMaxItem_);
	for (size_t i = 0; i < items_.size(); i++)
	{
		items_[i] = new Item();
		items_[i]->Initialize("Item", BaseObject::tagItem);
		items_[i]->SetIsActive(false);
		manager->AddGameObject(items_[i]);
	}

	for (Catapult* catapult : catapults_) {
		delete catapult;
	}
	catapults_.clear();
	catapults_.resize(kMaxCatapult_);
	for (size_t i = 0; i < catapults_.size(); i++)
	{
		catapults_[i] = new Catapult();
		catapults_[i]->Initialize("Catapult", BaseObject::tagCatapult);
		catapults_[i]->SetIsActive(false);
		manager->AddGameObject(catapults_[i]);
	}

	AddGloavalVariables();
	// ステージの設定を読み込む
	LoadStages();
}

void StageManager::Reset()
{
	for (size_t i = 0; i < items_.size(); i++)
		items_[i]->SetIsActive(false);
	for (size_t i = 0; i < catapults_.size(); i++)
		catapults_[i]->SetIsActive(false);
}

void StageManager::Update()
{
#ifdef _DEBUG

	DebugGUI();

#endif // _DEBUG

	// 現在のシーンの更新
	currentStage_->Update();
	gameTime_++;
}

void StageManager::SetStage(size_t stageIndex)
{
	if (kMaxStageNum_ <= stageIndex) {
		return;
	}
	nowStageNum_ = static_cast<int>(stageIndex);
	Reset();

#ifdef _DEBUG

	if (1) {
		currentStage_ = new DebugStage();
		currentStage_->commonInitialize();
		currentStage_->Initialize(infos_[stageIndex]);
		return;
	}

#endif // _DEBUG

	currentStage_ = new Stage();
	currentStage_->commonInitialize();
	currentStage_->Initialize(infos_[stageIndex]);

}

void StageManager::AddStageInfo(const BaseStage::StageInfo& info) {
	infos_.push_back(info);
	kMaxStageNum_ = static_cast<int32_t>(infos_.size());
}


int StageManager::GetUsedItem()const {
	int usedItemNum = 0;

	const std::vector<ItemInfo>& iInfo = infos_[nowStageNum_].itemInfo_;
	for (size_t i = 0; i < iInfo.size(); i++)
	{
		if (items_[i]->GetIsUsed()) {
			usedItemNum++;
		}
	}
	return usedItemNum;
}

void StageManager::AddGloavalVariables()
{
	// グループを作っておく
	globalVariables_->CreateGroup("Stage");
	globalVariables_->CreateGroup("StageInfo");

	globalVariables_->AddItem("Stage", "kMaxStageNum", kMaxStageNum_);
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
		infos_[i] = LoadInfo(i);
	}
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
		//iInfo[i].isRePop_ = static_cast<bool>(globalVariables_->GetIntValue(strStage, indexNum + "isRePop:" + std::to_string(i)));
		//iInfo[i].position_ = globalVariables_->GetVector3Value(strStage, indexNum + "ItemPosition:" + std::to_string(i));
		iInfo[i] = LoadItem(indexNum, std::to_string(i));
	}
	// カタパルト
	int kMaxCatapultNum = globalVariables_->GetIntValue(strStage, indexNum + "kMaxCatapultNum");
	// 読み込む
	std::vector<CatapultInfo>& cInfo = info.catapultInfo_;
	cInfo.resize(kMaxCatapultNum);
	for (size_t i = 0; i < cInfo.size(); i++)
	{
		cInfo[i] = LoadCatapult(indexNum, std::to_string(i));
	}

	return info;
}

ItemInfo StageManager::LoadItem(const std::string& indexNum, const std::string& infoIndex)
{
	ItemInfo iInfo{};
	iInfo.isRePop_ = static_cast<bool>(globalVariables_->GetIntValue("StageInfo", indexNum + "isRePop:" + std::to_string(i)));
	iInfo.position_ = globalVariables_->GetVector3Value("StageInfo", indexNum + "position:" + std::to_string(i));
	iInfo.popTime_ = globalVariables_->GetIntValue("StageInfo", indexNum + "popTime:" + std::to_string(i));
	return iInfo;
}


CatapultInfo StageManager::LoadCatapult(const std::string& indexNum, const std::string& infoIndex)
{
	CatapultInfo cInfo{};
	cInfo.theta_ = globalVariables_->GetFloatValue("StageInfo", indexNum + "theta:" + infoIndex);
	cInfo.length_ = globalVariables_->GetFloatValue("StageInfo", indexNum + "length:" + infoIndex);

	cInfo.isRePop_ = static_cast<bool>(globalVariables_->GetIntValue("StageInfo", indexNum + "CisRePop:" + infoIndex));
	cInfo.popTime_ = globalVariables_->GetIntValue("StageInfo", indexNum + "CPopTime" + infoIndex);
	return cInfo;
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
		globalVariables_->AddItem(strStage, indexNum + "popTime:" + std::to_string(i), iInfo[i].popTime_);
		globalVariables_->SetValue(strStage, indexNum + "popTime:" + std::to_string(i), iInfo[i].popTime_);
	}

	// カタパルトを保存
	int32_t kMaxCatapultNum = static_cast<int32_t>(info.catapultInfo_.size());
	globalVariables_->AddItem(strStage, indexNum + "kMaxCatapultNum", kMaxCatapultNum);
	globalVariables_->SetValue(strStage, indexNum + "kMaxCatapultNum", kMaxCatapultNum);
	std::vector<CatapultInfo>& cInfo = info.catapultInfo_;
	for (size_t i = 0; i < cInfo.size(); i++)
	{
		globalVariables_->AddItem(strStage, indexNum + "theta:" + std::to_string(i), cInfo[i].theta_);
		globalVariables_->SetValue(strStage, indexNum + "theta:" + std::to_string(i), cInfo[i].theta_);
		globalVariables_->AddItem(strStage, indexNum + "length:" + std::to_string(i), cInfo[i].length_);
		globalVariables_->SetValue(strStage, indexNum + "length:" + std::to_string(i), cInfo[i].length_);

		globalVariables_->AddItem(strStage, indexNum + "CisRePop:" + std::to_string(i), static_cast<int32_t>(cInfo[i].isRePop_));
		globalVariables_->SetValue(strStage, indexNum + "CisRePop:" + std::to_string(i), static_cast<int32_t>(cInfo[i].isRePop_));
		globalVariables_->AddItem(strStage, indexNum + "CpopTime:" + std::to_string(i), cInfo[i].popTime_);
		globalVariables_->SetValue(strStage, indexNum + "CpopTime:" + std::to_string(i), cInfo[i].popTime_);
	}

}

void StageManager::DebugGUI()
{
#ifdef _DEBUG

	ImGui::Begin("StageManager");

	ImGui::Text("StageNum:%d", infos_.size());
	ImGui::Text("kStageMaxNum:%d", kMaxStageNum_);
	ImGui::Separator();

	ImGui::Text("NowStageIndex:%d", nowStageNum_);
	if (ImGui::Button("ChangeStage")) {
		SetStage(nextStageNum_);
	}
	ImGui::SameLine();
	ImGui::InputInt("NextStageIndex", &nextStageNum_, 1, 5);
	if (nextStageNum_ < 0) {
		nextStageNum_ = 0;
	}
	ImGui::Separator();

	if (ImGui::Button("CommitAll")) {
		SaveStages();
	}
	if (ImGui::Button("CommitAs")) {
		kMaxStageNum_ = static_cast<int32_t>(infos_.size());
		SaveInfo(commitIndex_);
	}
	ImGui::SameLine();
	ImGui::InputInt("index", &commitIndex_, 1, 5);
	if (commitIndex_ < 0) {
		commitIndex_ = 0;
	}
	else if (infos_.size() - 1 < commitIndex_) {
		commitIndex_ = static_cast<int>(infos_.size()) - 1;
	}
	ImGui::Separator();
	ImGui::Separator();
	if (ImGui::Button("LoadAll")) {
		LoadStages();
	}
	if (ImGui::Button("LoadAs")) {
		AddStageInfo(LoadInfo(loadIndex_));
	}
	ImGui::SameLine();
	ImGui::InputInt("index", &loadIndex_, 1, 5);
	if (loadIndex_ < 0) {
		loadIndex_ = 0;
	}
	ImGui::Separator();

	if (ImGui::Button("PopBack")) {
		if (infos_.size() != 0) {
			infos_.pop_back();
		}
	}

	else if (infos_.size() - 1 < nextStageNum_) {
		nextStageNum_ = static_cast<int>(infos_.size()) - 1;
	}

	ImGui::End();

#endif // _DEBUG
}