#include "Ground.h"
#include "../../../Resource/Texture/TextureManager.h"

void Ground::Init()
{
	// 床トランスフォームの初期化
	floorTransform_.Init();
	// 床
	floorTransform_.translate_ = { 0.0f, -1.5f, 0.0f };
	floorTransform_.scale_ = { 1000.0f, 1.0f, 1000.0f };

	// AABBのコライダーを追加
	AddColliderAABB("Ground", &transform_.translate_, &transform_.scale_);
	// メッシュの追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// メッシュのテクスチャを変更
	meshes_[0]->texture_ = TextureManager::Load("./Resources", "SoilGround.png");
	
	// メッシュの追加
	AddMesh(&floorTransform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	// メッシュのテクスチャを変更
	meshes_[1]->texture_ = TextureManager::Load("./Resources", "GroundTex.png");

}

void Ground::Update()
{
	// メッシュのuvトランスフォームのスケールを床のスケールによって変更する
	meshes_[0]->material_.uvTransform_.scale_ = { transform_.scale_.x / 2.0f,  transform_.scale_.z / 2.0f, 1.0f };
}

void Ground::DisplayImGui()
{
	// トランスフォームのImGuiの表示
	transform_.DisplayImGui();
	
	// メッシュのUVトランスフォームの表示
	meshes_[0]->material_.uvTransform_.DisplayImGuiWithTreeNode("uvTransform");
}
