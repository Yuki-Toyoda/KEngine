#include "Player.h"

void Player::Init()
{
	// 移動ベクトル初期化
	velocity_ = { 0.0f,0.0f,0.0f };
	// 球のコライダーを追加
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);
}

void Player::Update()
{
	// 移動ベクトル初期化
	velocity_ = { 0.0f,0.0f,0.0f };

	velocity_ = InputManager::Move();
	transform_.translate_ = transform_.translate_ + (Math::Normalize(velocity_) * moveSpeed_);
	
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
}
