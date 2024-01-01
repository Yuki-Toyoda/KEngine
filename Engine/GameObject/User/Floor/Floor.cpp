#include "Floor.h"
#include "../../../Resource/Texture/TextureManager.h"

void Floor::Init()
{
	// メッシュの追加を行う
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// テクスチャの変更を行う
	meshes_[0]->texture_ = TextureManager::Load("./Resources", "GroundTex.png");

	// コライダーを追加する
	AddColliderOBB("Floor", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
}

void Floor::Update()
{

}

void Floor::DisplayImGui()
{
	transform_.DisplayImGui();
}
