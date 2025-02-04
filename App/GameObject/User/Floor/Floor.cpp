#include "Floor.h"
#include "Engine/Resource/Texture/TextureManager.h"

void Floor::Init()
{
	// メッシュの追加を行う
	AddNormalModel("Floor", &transform_, "./Engine/Resource/System/Box", "Box.obj");
	// テクスチャの変更を行う
	normalModels_["Floor"]->materials_[1].tex_ = TextureManager::Load("./Resources", "GroundTex.png");

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
