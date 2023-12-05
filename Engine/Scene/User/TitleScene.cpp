#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../Resource/Texture/TextureManager.h"

void TitleScene::Initialize()
{
	SampleBox* box = nullptr;
	box = GameObjectManager::GetInstance()->CreateInstance<SampleBox>("box", BaseObject::TagNone);
	box->transform_.translate_ = { 3.0f, 0.0f, 0.0f };
	box->meshes_[0]->texture_ = TextureManager::Load("White2x2.png");

	box = GameObjectManager::GetInstance()->CreateInstance<SampleBox>("box", BaseObject::TagPlayer);
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
