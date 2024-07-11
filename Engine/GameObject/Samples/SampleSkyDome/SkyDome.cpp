#include "SkyDome.h"
#include "../../../Resource/Texture/TextureManager.h"

void SkyDome::Init()
{
	// メッシュを追加
	AddNormalModel(&transform_, "./Engine/Resource/Samples/SkyDome", "SkyDome.obj", false);
	normalModels_[0]->materials_[1].tex_ = TextureManager::Load("./Resources/", "SkyTex.png");
}

void SkyDome::Update()
{
}

void SkyDome::DisplayImGui()
{
	transform_.DisplayImGui();
}
