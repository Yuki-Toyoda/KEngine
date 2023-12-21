#include "Player.h"

void Player::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");

	velocity_ = { 1.0f,1.0f,1.0f };
}

void Player::Update()
{
	if (InputManager::MoveLeft()) {
		//transform_.translate_.x += velocity_.x;
	}
}

void Player::DisplayImGui()
{
}
