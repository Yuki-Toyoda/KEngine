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
	isChain_ = true;
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
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isChain_ = false;
	}
	if (isChain_) {
		Move();
	}
	else {
		ChainDeleted();
	}
}

void Wepon::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("worldPos", &worldPos_.x);
}

void Wepon::Move()
{
	if (isMove_) {
		if (InputManager::RotateRight()) {
			rotateDirection_ = false;
		}
		if (InputManager::RotateLeft()) {
			rotateDirection_ = true;
		}
		if (rotateDirection_) {
			theta_ += 0.05f;
			transform_.rotate_.z += 0.05f;
		}
		else {
			theta_ -= 0.05f;
			transform_.rotate_.z -= 0.05f;
		}
	}
	transform_.translate_.x = std::cosf(theta_) * (distance_+transform_.scale_.x);
	transform_.translate_.y = std::sinf(theta_) * (distance_+transform_.scale_.y);
	if (lerpCount_ < 1.0f) {
		lerpCount_+=0.1f;
	}
	else {
		lerpCount_ = 1.0f;
	}
	distance_ = Math::Linear(lerpCount_, distance_, goalDistance_);
}

void Wepon::ChainDeleted()
{
	if (transform_.GetParent()) {
       transform_.translate_ = transform_.GetWorldPos();
	   transform_.SetParent(nullptr);
	}
	transform_.translate_.y -= 0.3f;

}

void Wepon::OnCollisionEnter(Collider* collider)
{
	collider;
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagEnemy) {
		
		if (lerpCount_ != 1.0f) {
			lerpCount_ = 1.0f;
			goalDistance_ = distance_ + 1.0f /2.5f;
			distance_ = Math::Linear(lerpCount_, distance_, goalDistance_);
		}
		lerpCount_ = 0.0f;
		goalDistance_ = distance_ + 1.0f / 2.5f;
	}
}
