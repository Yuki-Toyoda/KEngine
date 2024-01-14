#include "Ground.h"

void Ground::Init()
{
	AddColliderOBB("Ground", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	HP_ = 5;
}

void Ground::Update()
{
	if (HP_ <= 0) {
		isActive_ = false;
	}
	
}

void Ground::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragInt("HP", &HP_);
}

void Ground::Damage( Wepon* wepon)
{
	
		HP_ -= wepon->GetParentCount();
	
}
