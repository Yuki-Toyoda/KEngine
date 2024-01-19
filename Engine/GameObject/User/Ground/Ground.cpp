#include "Ground.h"

void Ground::Init()
{
	AddColliderOBB("Ground", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
}

void Ground::Update()
{
	
	
}

void Ground::DisplayImGui()
{
	transform_.DisplayImGui();
	
}

void Ground::Damage( Weapon* wepon)
{
	
	wepon;
	
}
