#include "Enemy.h"

void Enemy::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	transform_.scale_ = { 0.2f,0.2f,0.2f };
	worldPos_ = transform_.GetWorldPos();
	AddColliderSphere("Enemy",  &worldPos_,&transform_.scale_.x);
	isParent_ = false;
}

void Enemy::Update()
{
	worldPos_ = transform_.GetWorldPos();
	/*if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		transform_.SetParent(nullptr);
	}*/
	if (isParent_) {
		Vector3 world = transform_.GetWorldPos();
		Vector3 weponWorld = wepon_->transform_.GetWorldPos();

		velocity_ = Math::Normalize( weponWorld-world);
		velocity_ = velocity_ * 0.8f;
		transform_.translate_ += velocity_;
	}
	//isActive_ = true;
}

void Enemy::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("worldPos", &worldPos_.x);
}

void Enemy::OnCollisionEnter(Collider* collider)
{
	collider;
	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagWeapon) {
		if (GetObjectTag() != BaseObject::TagPlayer) {
			color_ = { 1.0f,0.0f,0.0f,1.0f };
			SetObjectTag(BaseObject::TagWeapon);
			Vector3 world = transform_.GetWorldPos();
			Vector3 weponWorld = wepon_->transform_.GetWorldPos();

			Vector3 pos = world - weponWorld;
			Matrix4x4 rotateMat = Math::MakeRotateXYZMatrix(wepon_->transform_.rotate_ * -1.0f);
			transform_.translate_ = Math::TransformNormal(pos, rotateMat);
			isParent_ = false;
			transform_.SetParent(&wepon_->transform_);
			//ペアレントしてる数を加算
			wepon_->AddParentCount();
		}
	}
	else if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagChain) {
		if (GetObjectTag() == BaseObject::TagEnemy) {
			color_ = { 1.0f,0.0f,0.0f,1.0f };
			isParent_ = true;

		}
	}
}
