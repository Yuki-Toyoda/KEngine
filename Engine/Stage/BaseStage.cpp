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

	player_ = static_cast<Player*>(gameObjectManager_->GetGameObject(BaseObject::tagPlayer).front());
}

void BaseStage::ApplyStageInfo()
{
	std::vector<Item*>& item = stageManager_->GetItems();
	item;
	for (size_t i = 0; i < stageInfo_.itemInfo_.size(); i++)
	{

	}
}
