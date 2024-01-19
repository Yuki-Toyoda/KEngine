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
	worldPos_ = transform_.translate_;
	// 移動ベクトル初期化
	velocity_ = { 0.0f,0.0f,0.0f };

	velocity_ = InputManager::Move();
	transform_.translate_ = transform_.translate_ + (Math::Normalize(velocity_) * moveSpeed_);
	
	
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
}

void Player::OnCollisionEnter(Collider* collider)
{
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagRubble) {
		absorptionCount_++;
		float addSize = absorptionCount_ * scaleForce_;
		transform_.translate_.y += addSize;
		transform_.scale_ = { transform_.scale_.x + addSize,transform_.scale_.y + addSize,transform_.scale_.z + addSize };

	}
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagMeteor) {
		
	}
}

void Player::Damage()
{
}
