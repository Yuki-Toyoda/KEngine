#include "Player.h"

void Player::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	velocity_ = { 0.0f,0.0f,0.0f };
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);
}

void Player::Update()
{
	velocity_ = { 0.0f,0.0f,0.0f };

	velocity_ = InputManager::Move();
	transform_.translate_ = transform_.translate_ + (Math::Normalize(velocity_) * moveSpeed_);
	
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
}
