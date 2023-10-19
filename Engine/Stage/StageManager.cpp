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

	items_.clear();
	items_.resize(kMaxItem_);
	for (size_t i = 0; i < items_.size(); i++)
	{
		items_.push_back(new Item);
		items_.back()->Initialize("Item", BaseObject::tagItem);
		items_.back()->SetIsActive(false);
	}
	// ステージの設定を読み込む
	LoadStages();
}

void StageManager::Reset()
{
	for (size_t i = 0; i < kMaxItem_; i++)
	{
		items_.back()->Initialize("Item", BaseObject::tagItem);
		items_.back()->SetIsActive(false);
	}
}

void StageManager::Update()
{
#ifdef _DEBUG

	//// FPSカウンターの表示
	//ImGui::Begin("Control panel");
	//ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	//ImGui::End();

#endif // _DEBUG

	// 現在のシーンの更新
	currentStage_->Update();

}

void StageManager::SetStage(size_t index)
{
	currentStage_ = new FirstStage();
	currentStage_->Initialize(infos_[index]);
}

void StageManager::LoadStages()
{
	// 作ったステージの個数を取得
	kMaxStaegNum_ = globalVariables_->GetIntValue("Stage", "kMaxStageNum");
	// ステージの回数分読み込む
	infos_.resize(kMaxStaegNum_);
	for (size_t i = 0; i < infos_.size(); i++)
	{
		infos_.push_back(LoadInfo(i));
	}

}

StageInfo StageManager::LoadInfo(size_t num)
{
	// Item の位置、リポップフラグ
	// クリアゲージ
	// 敵
	// カタパルトの個数、位置
	StageInfo info{};
	std::string index = std::to_string(num);
	std::string strStage = "Stage" + index;
	info.gearInfo_.clearCondition_ = globalVariables_->GetFloatValue(strStage, "Condition");
	// アイテムの個数を取得
	int kMaxInfoNum = globalVariables_->GetIntValue(strStage, "kMaxInfoNum");
	// アイテムを読み込む
	std::vector<ItemInfo>& iInfo = info.itemInfo_;
	iInfo.resize(kMaxInfoNum);
	for (size_t i = 0; i < iInfo.size(); i++) {
		iInfo[i].isRePop_ = static_cast<bool>(globalVariables_->GetIntValue(strStage, "isRePop" + i));
		iInfo[i].position_ = globalVariables_->GetVector3Value(strStage, "ItemPosition" + i);
	}
	return info;
}

void StageManager::SaveStages()
{
	// ステージの数だけ書き出す
	for (size_t i = 0; i < infos_.size(); i++)
	{
		SaveInfo(i);
	}
}

void StageManager::SaveInfo(size_t num)
{
	std::string index = std::to_string(num);
	std::string strStage = "Stage" + index;
	StageInfo& info = infos_[num];
	globalVariables_->AddItem(strStage, "Condition", info.gearInfo_.clearCondition_);
	int32_t kMaxInfoNum = static_cast<int32_t>(info.itemInfo_.size());
	globalVariables_->AddItem(strStage, "kMaxInfoNum", kMaxInfoNum);
	std::vector<ItemInfo>& iInfo = info.itemInfo_;
	for (size_t i = 0; i < kMaxInfoNum; i++)
	{
		globalVariables_->AddItem(strStage, "isRePop", static_cast<int32_t>(iInfo[i].isRePop_));

	}
}
