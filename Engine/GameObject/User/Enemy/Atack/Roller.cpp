#include "Roller.h"

void Roller::Init()
{
	velocity_ = { 1.0f,0.0f,0.0f };
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	AddColliderOBB("Roller", &transform_.scale_, &transform_.rotate_, &transform_.translate_);
	transform_.scale_ = { 8.0f,1.0f,1.0f };
	transform_.rotate_.y = std::atan2(velocity_.x, velocity_.y);
	transform_.translate_.y = 2.0f;
	color_ = { 1.0f,0.0f,0.0f,1.0f };
	
}

void Roller::Update()
{
	//移動の向きに合わせて回転
	
	transform_.rotate_.y = std::atan2(velocity_.x, velocity_.z);

	if (velocity_.x > velocity_.x) {
		
		transform_.rotate_.x = 0.0f;
		if (velocity_.x >= 0.0f) {
			transform_.rotate_.z += 0.1f;
		}
		else {
			transform_.rotate_.z -= 0.1f;
		}
	}
	else {
		transform_.rotate_.z = 0.0f;
		if (velocity_.z >= 0.0f) {
			transform_.rotate_.x += 0.1f;
		}
		else {
			transform_.rotate_.x -= 0.1f;
		}
	}
	transform_.translate_ += Math::Normalize( velocity_) * moveAcceleration_;
	if (std::abs(transform_.translate_.x) >= 60.0f || std::abs(transform_.translate_.z) >= 35.0f) {
		Destroy();
		return;
	}
}

void Roller::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("velocity", &velocity_.x, 0.1f);
}

void Roller::OnCollisionEnter(Collider* collider)
{
	collider;
}
