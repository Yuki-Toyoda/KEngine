#include "Weapon.h"
#include "../../GameObjectManager.h"
void Weapon::Init()
{
	audio_ = Audio::GetInstance();
	soundHandleslam_ = audio_->LoadWave("slam.wav");
	soundHandleDamage_= audio_->LoadWave("damege.wav");
	AddMesh(&transform_, color_, "./Resources/WeaponCube", "weaponCube.obj");
	AddColliderSphere("Wepon", &worldPos_, &transform_.scale_.x);
	distance_ = 5.0f;
	transform_.translate_.x +=distance_;
	theta_ = 0.0f;
	rotateDirection_ = false;
	worldPos_ = transform_.GetWorldPos();
	isMove_ = true;
	isChain_ = true;
	behavior_ = Behavior::kRoot;
	isAtack_ = false;
	parentCount_ = 0;
	goalDistance_ = distance_;
	isBreak_ = false;
}

void Weapon::Update()
{
	if (Input::GetInstance()->PushKey(DIK_RETURN)) {
		isBreak_ = true;
	}
	if (behaviorRequest_) {
		behavior_ = behaviorRequest_.value();
		behaviorRequest_ = std::nullopt;
	}
	switch (behavior_) {
	case Behavior::kAtack:
		if (!isAtackEnd_) {
			Atack();
			//Move();
			
		}
		break;
	case Behavior::kRoot:
		if (Input::GetInstance()->PushKey(DIK_LSHIFT)) {
			if (isMove_) {
				isMove_ = false;
			}
			else {
				isMove_ = true;
			}
		}
		
		if (isChain_) {

			Move();
		}
		else {
			ChainDeleted();
		}
		if (InputManager::Atack()) {
			isAtack_ = true;
		}
		if (isCommbo_) {
			commbCount_++;
			if (kMaxCommbCount_ <= commbCount_) {
				isCommbo_ = false;
				commbCount_ = 0;
			}
		}
		break;
	default:
		break;
	}
	worldPos_ = transform_.GetWorldPos();
	if (GetBehavior() == Behavior::kAtack) {
		colliders_.front()->SetIsActive(false);
	}
	else {
		colliders_.front()->SetIsActive(true);
	}
	if (parentCount_ < 5) {
		size_ = low;
	}
	else if (parentCount_ < 10) {
		size_= mid;
	}
	else {
		size_ = high;
	}
}

void Weapon::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("worldPos", &worldPos_.x);
	ImGui::DragFloat("theta", &theta_);

	ImGui::DragFloat("Distance", &distance_);
	ImGui::InputInt("parentCount", &parentCount_);
}

void Weapon::Reset()
{
	distance_ = 5.0f;
	transform_.translate_.x += distance_;
	transform_.rotate_ = { 0.0f,0.0f,0.0f };
	theta_ = 0.0f;
	rotateDirection_ = false;
	worldPos_ = transform_.GetWorldPos();
	isChain_ = true;
	behavior_ = Behavior::kRoot;
	isAtack_ = false;
	parentCount_ = 0;
	goalDistance_ = distance_;
	isBreak_ = false;
	isAtackEnd_ = false;
}

void Weapon::Move()
{
	if (isMove_) {
		if (InputManager::ChangeRotate()) {
			if (rotateDirection_) {
				rotateDirection_ = false;
			}
			else {
				rotateDirection_ = true;
			}
		}
		if (rotateDirection_) {
			if(theta_>=(2.0f*3.14159265f)){
				theta_ = 0.0f;
			}
			if (moveRotateForce_ <= kMoveRotateForce_) {
				moveRotateForce_ += 0.01f;
			}
			if (!isCommbo_ && moveRotateForce_ > kMoveRotateForce_) {
				moveRotateForce_ -= 0.01f;
			}
			
		}
		else {

			if (theta_ <= -(2.0f * 3.14159265f)) {
				theta_ = 0.0f;
			}
			if (moveRotateForce_ >= -kMoveRotateForce_) {
				moveRotateForce_ -= 0.01f;
			}
			if (!isCommbo_ && moveRotateForce_ < -kMoveRotateForce_) {
				moveRotateForce_ += 0.01f;
			}
			
		}
	}
	theta_ += moveRotateForce_;
	transform_.rotate_.z += moveRotateForce_;
	transform_.translate_.x = std::cosf(theta_) * (distance_+transform_.scale_.x);
	transform_.translate_.y = std::sinf(theta_) * (distance_+transform_.scale_.y);
	if (lerpCount_ < 1.0f) {
		lerpCount_+=0.1f;
	}
	else {
		lerpCount_ = 1.0f;
	}
	distance_ = Math::Linear(lerpCount_, distance_, goalDistance_);
	
	
		if (rotateDirection_) {
			if (theta_ >= (0.5f * 3.14159265f)) {
				if (theta_ <= (0.51f * 3.14159265f)) {
					audio_->PlayWave(soundHandleslam_);
					
				}
				if (isAtack_) {
					behaviorRequest_ = Behavior::kAtack;
				}
			}
		}
		else {
			if (theta_ <= -(1.49f * 3.14159265f)) {
				if (theta_ >= -(1.5f * 3.14159265f)) {
					audio_->PlayWave(soundHandleslam_);
				}
				if (isAtack_) {
					behaviorRequest_ = Behavior::kAtack;
				}
			}
		}
	
	
}

void Weapon::ChainDeleted()
{
	if (transform_.GetParent()) {
       transform_.translate_ = transform_.GetWorldPos();
	   transform_.SetParent(nullptr);
	}
	transform_.translate_.y -= 0.3f;
	if (!isChain_ && transform_.translate_.y <= -20.0f) {
		isBreak_ = true;
	}
}

void Weapon::Atack()
{
	
	goalDistance_ += 0.3f;
	if (rotateDirection_) {
		if (theta_ >= (1.5f * 3.14159265f)&& theta_ <= (1.75f * 3.14159265f)) {
			//ChainDeleted();
			//isChain_ = false;
			isAtackEnd_ = true;
			audio_->PlayWave(soundHandleDamage_);
		}
	}
	else {
		if (theta_ <= -(0.5f * 3.14159265f)&& theta_ >= -(0.75f * 3.14159265f)) {
			//ChainDeleted();
			//isChain_ = false;
			isAtackEnd_ = true;
			audio_->PlayWave(soundHandleDamage_);
		}
	}
	if (rotateDirection_) {
		if (theta_ >= (2.0f * 3.14159265f)) {
			theta_ = 0.0f;
		}
		theta_ += kAtackRotateForce_;
		transform_.rotate_.z += kAtackRotateForce_;
	}
	else {
		if (theta_ <= -(2.0f * 3.14159265f)) {
			theta_ = 0.0f;
		}
		theta_ -= kAtackRotateForce_;
		transform_.rotate_.z -= kAtackRotateForce_;
	}
	transform_.translate_.x = std::cosf(theta_) * (distance_ + transform_.scale_.x);
	transform_.translate_.y = std::sinf(theta_) * (distance_ + transform_.scale_.y);
}

void Weapon::Commbo()
{

	if (isCommbo_) {
		if (rotateDirection_) {
			moveRotateForce_ += AddMoveRotateForce_;
		}
		else {
			moveRotateForce_ -= AddMoveRotateForce_;
		}
	}
	else {
		isCommbo_ = true;
		
	}
	commbCount_ = 0;
}

void Weapon::OnCollisionEnter(Collider* collider)
{
	collider;
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagEnemy) {
		
	/*	if (lerpCount_ != 1.0f) {
			lerpCount_ = 1.0f;
			goalDistance_ = distance_ + 1.0f /2.5f;
			distance_ = Math::Linear(lerpCount_, distance_, goalDistance_);
		}
		lerpCount_ = 0.0f;
		goalDistance_ = distance_ + 1.0f / 2.5f;*/
	}
}
