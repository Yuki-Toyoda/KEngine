#include "TitleScene.h"
#include "Engine/Scene/SceneManager.h"
#include "Engine/Resource/Texture/TextureManager.h"

void TitleScene::Init()
{
	
}

void TitleScene::Update()
{

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
#endif // _DEBUG
}
