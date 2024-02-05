#include "ObjectSceneManager.h"
#include "../../../Resource/Texture/TextureManager.h"

void ObjectSceneManager::Init()
{
	AddSprite("UI", { 760.0f, 225.0f }, { 512.0f, 512.0f }, TextureManager::Load("SceneTex1.png"));
}

void ObjectSceneManager::Update()
{
	
}

void ObjectSceneManager::DisplayImGui()
{
	// スプライトのImGuiを表示する
	sprites_[0]->DisplayImGui();
}
