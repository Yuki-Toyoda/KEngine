#include "Wepon.h"
#include "../../GameObjectManager.h"
void Wepon::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	distance_ = 5.0f;
	transform_.translate_.x +=distance_;
	theta_ = 0.0f;
	rotateDirection_ = false;
	worldPos_ = transform_.GetWorldPos();
	AddColliderSphere("Wepon", &worldPos_,&transform_.scale_.x);
}

void Wepon::Update()
{
	worldPos_ = transform_.GetWorldPos();
	if (Input::GetInstance()->PushKey(DIK_LSHIFT)) {
		if (isMove_) {
			isMove_ = false;
		}
		else {
			isMove_ = true;
		}
	}
	if (isMove_) {
		Move();
	}
}

void Wepon::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("worldPos", &worldPos_.x);
}

void Wepon::Move()
{
	if (InputManager::RotateRight()) {
		rotateDirection_ = false;
	}
	if (InputManager::RotateLeft()) {
		rotateDirection_ = true;
	}
	if (rotateDirection_) {
		theta_ += 0.1f;
		transform_.rotate_.z += 0.1f;
	}
	else {
		theta_ -= 0.1f;
		transform_.rotate_.z -= 0.1f;
	}
	transform_.translate_.x = std::cosf(theta_) * distance_;
	transform_.translate_.y = std::sinf(theta_) * distance_;
	if (lerpCount_ < 1.0f) {
		lerpCount_+=0.1f;
	}
	else {
		lerpCount_ = 1.0f;
	}
	distance_ = Math::Linear(lerpCount_, distance_, goalDistance_);
}

void Wepon::OnCollisionEnter(Collider* collider)
{
	collider;
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagEnemy) {
		parentCount_++;
		if (lerpCount_ != 1.0f) {
			lerpCount_ = 1.0f;
			goalDistance_ = distance_ + (float)parentCount_ / 4.0f;
			distance_ = Math::Linear(lerpCount_, distance_, goalDistance_);
		}
		lerpCount_ = 0.0f;
		goalDistance_ = distance_ + (float)parentCount_ / 4.0f;
	}
}
