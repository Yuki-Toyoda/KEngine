#include "TitleScene.h"
#include "../SceneManager.h"

void TitleScene::Initialize()
{
}

void TitleScene::Update()
{

	// 右シフトが押されたら次のシーンへ
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

}
