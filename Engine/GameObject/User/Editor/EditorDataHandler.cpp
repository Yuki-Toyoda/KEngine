#include "GameEditor.h"

/// <summary>
/// リロード関数
/// </summary>
void GameEditor::DataReaload()
{
	// Jsonからの情報を再取得
	GameDataManager::GetInstance()->ClearGroup();
	GameDataManager::GetInstance()->LoadFiles();
	gameManager_ = nullptr;
	// オブジェクト内容初期化
	gameObjectManager_->Init();
	// カメラ設定
	CreateCamera();
	// リストクリア
	// 隕石
	meteors_.clear();
	// 突き上げ
	pushUps_.clear();
	// ローラー
	rollers_.clear();
	// Jsonの情報による初期化
	ParameterInitialize();
}

void GameEditor::SaveAll()
{

	SaveSingle();
	SaveRoller();
	SaveMulti();
	SavePushUp();

}

void GameEditor::SaveSingle() 
{
	// 名前
	HierarchicalName names;
	std::string max = "MaxCount";
	// 基本情報
	std::string param = kParamSectionName;
	names.kSection = param;
	names.kGroup = dataManager_->GetSingleAttack(stageNumber[kSingle]);
	// 最大数の設定
	dataManager_->SetValue(names, max, meteorCounter_);
	dataManager_->SetValue(names, "Scale", singleSize_);
	dataManager_->SetValue(names, "Distance", respawnDistance_);
	dataManager_->SetValue(names, "IsGravity", isGravity_);
	// メテオ
	SaveObject(meteors_, names.kGroup, 0);
}

void GameEditor::SavePushUp()
{
	// 名前
	HierarchicalName names;
	std::string max = "MaxCount";
	// 基本情報
	std::string param = kParamSectionName;
	names.kSection = param;
	// 突き上げ
	// ここを拡張
	names.kGroup = dataManager_->GetPushUpAttack(stageNumber[kPushUp]);
	dataManager_->SetValue(names, max, pushUpCounter_);
	dataManager_->SetValue(names, "Scale", pushUpSize_);

	// 突き上げ
	SaveObject(pushUps_, names.kGroup, 1);

}

void GameEditor::SaveRoller()
{
	// 名前
	HierarchicalName names;
	std::string max = "MaxCount";
	// 基本情報
	std::string param = kParamSectionName;
	names.kSection = param;
	// ローラー
	names.kGroup = dataManager_->GetRollerAttack(stageNumber[kRoller]);
	dataManager_->SetValue(names, max, rollerCounter_);
	dataManager_->SetValue(names, "Scale", rollerSize_);
	counter_ = 0;
	for (Roller* object : rollers_) {
		// 名前
		std::string groupPath = names.kGroup;
		// セクションパス
		std::string sectionPath = dataManager_->kObjectNames[2] + std::to_string(counter_);
		// アイテムパス
		std::string keyPath = "Direct";
		dataManager_->AddItem({ groupPath,sectionPath }, keyPath, object->GetVelocity());
		dataManager_->SetValue({ groupPath,sectionPath }, keyPath, object->GetVelocity());
		counter_++;
	}

	// ローラー
	SaveObject(rollers_, names.kGroup, 2);

}

void GameEditor::SaveMulti()
{
	// 名前
	HierarchicalName names;
	std::string max = "MaxCount";
	// 基本情報
	std::string param = kParamSectionName;
	names.kSection = param;
	// 追尾
	names.kGroup = dataManager_->GetMultiAttack(stageNumber[kMulti]);
	dataManager_->SetValue(names, max, kMaxCount_);
	dataManager_->SetValue(names, "CoolTime", coolTime_);
	dataManager_->SetValue(names, "Scale", multiSize_);
	// ファイル保存
	dataManager_->SaveData(names.kGroup);
}

void GameEditor::ReleadTable() {
	tableNames_.clear();
	numberStrings_.clear();
	std::string group = "TableData";
	std::string section = "TableList" + std::to_string(nowTableNumber_);
	dataManager_->AddItem<std::list<std::string>>({ group,section }, "ActionList", tableNames_);
	dataManager_->AddItem<std::list<int>>({ group,section }, "NumberList", tableNumbers_);
	tableNames_ = dataManager_->GetValue<std::list<std::string>>({ group,section }, "ActionList");
	tableNumbers_ = dataManager_->GetValue<std::list<int>>({ group,section }, "NumberList");
	numberStrings_ = this->ConvertToStringList(tableNumbers_);
}