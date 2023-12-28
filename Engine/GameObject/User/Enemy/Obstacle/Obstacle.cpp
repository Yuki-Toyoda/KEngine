#include "Obstacle.h"

void Obstacle::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	color_ = { 0.0f,1.0f,0.0f,1.0f };
	AddColliderSphere("Enemy", &transform_.translate_, &transform_.scale_.x);
}

void Obstacle::Update()
{
}

void Obstacle::DisplayImGui()
{
	transform_.DisplayImGui();
}
