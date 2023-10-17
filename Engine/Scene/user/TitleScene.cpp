#include "TitleScene.h"

void TitleScene::Initialize()
{

}

void TitleScene::Update()
{
	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		nextScene_ = new GameScene();
		gameObjectManager_->Initialize();
	}
}
