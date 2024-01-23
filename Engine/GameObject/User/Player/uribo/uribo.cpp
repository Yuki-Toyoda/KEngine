#include "uribo.h"

void Uribo::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	AddColliderOBB("Roller", &collisionScaele_, &transform_.rotate_, &transform_.translate_);
	collisionScaele_ = { 10.0f,10.0f,10.0f };
	transform_.translate_.y = 2.0f;
	transform_.translate_.x = 30.0f;
	hitPoint_ = defaultHP_;
}

void Uribo::Update()
{
	hitPoint_ -= decrementHP;
	color_ = { 0.0f,0.0f,1.0f,1.0f };
}

void Uribo::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("collisionScale", &collisionScaele_.x);
	ImGui::DragInt("defaultHP", &defaultHP_);
	ImGui::DragInt("HP", &hitPoint_);
}

void Uribo::OnCollision(Collider* collider)
{
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagPlayer) {
		color_ = { 1.0f,1.0f,0.0f,1.0f };
	}
	
}

void Uribo::Heal(int healPower)
{
	hitPoint_ += healPower;
}
