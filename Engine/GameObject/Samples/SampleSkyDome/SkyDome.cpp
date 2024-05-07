#include "SkyDome.h"
#include "../../../Resource/Texture/TextureManager.h"

void SkyDome::Init()
{
	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/SkyDome", "SkyDome.obj", false);
	//meshes_[0]->texture_ = TextureManager::Load("./Resources", "SkyTex.png");
}

void SkyDome::Update()
{
}

void SkyDome::DisplayImGui()
{
	transform_.DisplayImGui();
}
