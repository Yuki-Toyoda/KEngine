#include "Enemy.h"

void Enemy::Init()
{
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Box", "Box.obj");
	AddColliderSphere("Enemy", &worldPos_, &transform_.scale_.x);
	transform_.scale_ = { 0.2f,0.2f,0.2f };
	worldPos_ = transform_.GetWorldPos();
	
	isParent_ = false;
}

void Enemy::Update()
{
	worldPos_ = transform_.GetWorldPos();
	//鎖とぶつかった場合
	if (isParent_) {
		Vector3 world = transform_.GetWorldPos();
		Vector3 weponWorld = wepon_->transform_.GetWorldPos();
		//当たった場所から武器の方向に飛ばす
		velocity_ = Math::Normalize( weponWorld-world);
		velocity_ = velocity_ * 0.8f;
		transform_.translate_ += velocity_;
	}
	
}

void Enemy::DisplayImGui()
{
	transform_.DisplayImGui();
	ImGui::DragFloat3("worldPos", &worldPos_.x);
}

void Enemy::Reset()
{
	transform_.scale_ = { 0.2f,0.2f,0.2f };
	worldPos_ = transform_.GetWorldPos();

	isParent_ = false;
	SetObjectTag(BaseObject::TagEnemy);
	transform_.SetParent(nullptr);
	transform_.translate_ = startTranslate_;
}

void Enemy::OnCollisionEnter(Collider* collider)
{
	collider;
	//武器とぶつかった

	if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagWeapon) {
		if (GetObjectTag() != BaseObject::TagPlayer) {
			color_ = { 1.0f,0.0f,0.0f,1.0f };
			SetObjectTag(BaseObject::TagWeapon);
			//ワールドポジションを計算
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
	//鎖とぶつかった
	else if (collider->GetGameObject()->GetObjectTag() == BaseObject::TagChain) {
		if (GetObjectTag() == BaseObject::TagEnemy) {
			color_ = { 1.0f,0.0f,0.0f,1.0f };
			
			isParent_ = true;

		}
	}
}
