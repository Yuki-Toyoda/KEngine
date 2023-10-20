#include "BaseStage.h"
#include "StageManager.h"
#include "../GameObject/Item/Item.h"

void BaseStage::commonInitialize()
{
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// 入力取得
	//input_ = Input::GetInstance();
	stageManager_ = StageManager::GetInstance();

	globalVariables_ = GlobalVariables::GetInstance();

	//player_ = static_cast<Player*>(gameObjectManager_->GetGameObject(BaseObject::tagPlayer).front());

	gearCondition_ = 0;
	usedItem_ = 0;
}

bool BaseStage::GetIsClear() const
{
	// 歯車の回転条件
	bool gearClear = false;
	// 設定した値以上回っていたら
	if (stageInfo_.gearInfo_.clearCondition_ <= gearCondition_) {
		gearClear = true;
	}
	// アイテムでのクリア条件
	bool itemClear = false;
	std::vector<Item*>& items = stageManager_->GetItems();	
	const std::vector<ItemInfo>& iInfo = stageInfo_.itemInfo_;
	// 一つでも使われていなかったらクリアではない
	for (size_t i = 0; i < iInfo.size(); i++)
	{
		itemClear = items[i]->GetIsUsed();
	}

	return gearClear && itemClear;
}

void BaseStage::ApplyStageInfo()
{
	// ステージのクリア条件は設定しなくてもいい
	// アイテムの初期化
	stageManager_->Reset();
	// アイテムの情報を設定する
	std::vector<Item*>& item = stageManager_->GetItems();
	std::vector<ItemInfo>& iInfo = stageInfo_.itemInfo_;
	for (size_t i = 0; i < iInfo.size(); i++)
	{
		/*
		item[i]->SetIsRePop(iInfo[i].isRePop_);
		item[i]->SetPosition(iInfo[i].position_);
		item[i]->SetPopTime(iInfo[i].popTime_);
		item[i]->SetIsActive(true);*/
		item[i]->SetItemInfo(iInfo[i]);
	}
}

