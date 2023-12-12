#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Resource/Texture/TextureManager.h"

void TitleScene::Init()
{
	SampleBox* box = nullptr;
	box = GameObjectManager::GetInstance()->CreateInstance<SampleBox>("box", BaseObject::TagNone);
	box->transform_.translate_ = { 0.0f, 0.0f, 0.0f };

	//box = GameObjectManager::GetInstance()->CreateInstance<SampleBox>("box", BaseObject::TagPlayer);
}

void TitleScene::Update()
{
	ImGui::Begin("Generate");
	if (ImGui::Button("Generate Plane")) {
		SampleBox* box = nullptr;
		box = GameObjectManager::GetInstance()->CreateInstance<SampleBox>("box", BaseObject::TagPlayer);
	}
	ImGui::End();

	// デバッグ時のみ特定のキーでシーン遷移
#ifdef _DEBUG
	// デバッグ遷移
	if (input_->TriggerKey(DIK_RSHIFT)) {
		BaseScene* nextScene = new GameScene();
		SceneManager::GetInstance()->SetNextScene(nextScene);
	}
#endif // _DEBUG
}
