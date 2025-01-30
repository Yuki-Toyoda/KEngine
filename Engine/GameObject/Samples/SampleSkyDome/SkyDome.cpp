#include "SkyDome.h"
#include "../../../Resource/Texture/TextureManager.h"

void SkyDome::Init()
{
	// メッシュを追加
	AddNormalModel("SkyDome", &transform_, "./Engine/Resource/System/SkyDome", "SkyDome.obj", false);
	//meshes_[0]->material_->tex_ = TextureManager::Load("./Engine/Resource/System/SkyDome", "Sky.png");
}

void SkyDome::Update()
{
}

void SkyDome::DisplayImGui()
{
	transform_.DisplayImGui();
}
