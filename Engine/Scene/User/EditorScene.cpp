#include "EditorScene.h"
#include "../SceneManager.h"

void EditorScene::Init()
{
	editor_ = std::make_unique<GameEditor>();
	editor_->Init();
}

void EditorScene::Update()
{

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}

#endif // _DEBUG
	
	editor_->Update();

}
