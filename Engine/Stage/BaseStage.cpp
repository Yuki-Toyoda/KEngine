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
}