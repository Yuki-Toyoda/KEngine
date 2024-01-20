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
	hitCollTimer_.Update();
	prevPos_ = worldPos_;
	worldPos_ = transform_.translate_;
	
	//// 移動ベクトル初期化
	//velocity_ = { 0.0f,0.0f,0.0f };

	velocity_ = InputManager::Move(velocity_);
	transform_.translate_ = transform_.translate_ + (Math::Normalize(velocity_) /** moveSpeed_*/);
	if (transform_.translate_.x+transform_.scale_.x >= ground_->transform_.scale_.x|| transform_.translate_.x - transform_.scale_.x <= -ground_->transform_.scale_.x) {
		velocity_.x =0.0f;
		transform_.translate_.x = prevPos_.x;
	}
	if (transform_.translate_.z + transform_.scale_.z >= ground_->transform_.scale_.z || transform_.translate_.z - transform_.scale_.z <= -ground_->transform_.scale_.z) {
		velocity_.z =0.0f;
		transform_.translate_.z = prevPos_.z;
	}
	SubtractVelocity();
	
}

void Player::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat("deceleration rate", &decelerationRate, 0.01f);
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
		Damage();
	}
}

void Player::Damage()
{
	if (hitCollTimer_.GetIsFinish()) {
		hitPoint_--;
		hitCollTimer_.Start(hitCoolTime_);
	}

}
void Player::SubtractVelocity()
{
	velocity_ = velocity_ * decelerationRate;
	if (std::abs(velocity_.x) <= 0.01f) {
		velocity_.x = 0.0f;
	}
	if (std::abs(velocity_.z) <= 0.01f) {
		velocity_.z = 0.0f;
	}
}