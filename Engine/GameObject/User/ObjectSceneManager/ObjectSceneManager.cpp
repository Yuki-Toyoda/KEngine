#include "ObjectSceneManager.h"
#include "../../../Resource/Texture/TextureManager.h"

void ObjectSceneManager::Init()
{
	// スプライト追加
	AddSprite("UI", { 760.0f, 200.0f }, { 512.0f, 512.0f }, TextureManager::Load("SceneTex1.png"));
	AddSprite("UI2", { 760.0f, 200.0f }, { 512.0f, 256.0f }, TextureManager::Load("SceneTex2.png"));
}

void ObjectSceneManager::Update()
{

}

void ObjectSceneManager::DisplayImGui()
{
	// スプライトのImGuiを表示する
	sprites_[0]->DisplayImGui();
}

void ObjectSceneManager::SetSceneNo(bool scene)
{
	if (scene) {
		// フラグによって表示内容を変える
		sprites_[0]->SetIsActive(false);
		sprites_[1]->SetIsActive(true);

		sprites_[1]->translate_.y = 0.0f;
	}
	else {
		// フラグによって表示内容を変える
		sprites_[1]->SetIsActive(false);
		sprites_[0]->SetIsActive(true);
	}
}
