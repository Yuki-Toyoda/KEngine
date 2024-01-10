#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Resource/Texture/TextureManager.h"

void TitleScene::Init()
{
	
}

void TitleScene::Update()
{

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}
