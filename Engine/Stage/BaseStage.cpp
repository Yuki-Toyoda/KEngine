#include "BaseStage.h"
#include "StageManager.h"

void BaseStage::commonInitialize()
{
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// 入力取得
	//input_ = Input::GetInstance();
	stageManager_ = StageManager::GetInstance();

	globalVariables_ = GlobalVariables::GetInstance();

	//player_ = static_cast<Player*>(gameObjectManager_->GetGameObject(BaseObject::tagPlayer).front());
}

void BaseStage::ApplyStageInfo()
{
	// ステージのクリア条件は設定しなくてもいい
	// アイテムの情報を設定する
	std::vector<Item*>& item = stageManager_->GetItems();
	std::vector<ItemInfo>& iInfo = stageInfo_.itemInfo_;
	for (size_t i = 0; i < iInfo.size(); i++)
	{
		item[i]->SetIsRePop(iInfo[i].isRePop_);
		item[i]->SetPosition(iInfo[i].position_);
		item[i]->SetIsActive(true);
		//gameObjectManager_->AddGameObject(item[i]);
	}
}
