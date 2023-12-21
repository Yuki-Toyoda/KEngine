#include "Player.h"

void Player::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	velocity_ = { 0.0f,0.0f,0.0f };
}

void Player::Update()
{
	velocity_ = { 0.0f,0.0f,0.0f };
	if (InputManager::MoveUp()) {
		velocity_.y = 1.0f;
	}
	if (InputManager::MoveDown()) {
	   velocity_.y = -1.0f ;
	}
	if (InputManager::MoveLeft()) {
		velocity_.x  =  -1.0f ;
	}
	if (InputManager::MoveRight()) {
		velocity_.x = 1.0f;
	}
	
	transform_.translate_ = transform_.translate_ + (Math::Normalize(velocity_) * moveSpeed_);
	
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
}
