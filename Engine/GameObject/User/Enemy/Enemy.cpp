#include "Enemy.h"

void Enemy::Init()
{
	// 各種ワールドトランスフォームの初期化
	bodyTransform_.Init();
	// 親子付け
	bodyTransform_.SetParent(&transform_, 0b011);
	headTransform_.Init();
	headTransform_.SetParent(&bodyTransform_, 0b111);
	headTransform_.translate_ = { 0.0f, 1.1f, 0.0f };
	armTransform_R_.Init();
	armTransform_R_.SetParent(&bodyTransform_, 0b111);
	armTransform_R_.translate_ = { 0.0f, 1.0f, 0.0f };
	armTransform_L_.Init();
	armTransform_L_.SetParent(&bodyTransform_, 0b111);
	armTransform_L_.translate_ = { 0.0f, 1.0f, 0.0f };

	colliderTransform_.Init();

	// メッシュを追加
	AddMesh(&bodyTransform_, color_, "./Resources/Enemy", "Body.obj");
	AddMesh(&headTransform_, color_, "./Resources/Enemy", "Head.obj");
	AddMesh(&armTransform_R_, color_, "./Resources/Enemy", "Arm_R.obj");
	AddMesh(&armTransform_L_, color_, "./Resources/Enemy", "Arm_L.obj");
	AddMesh(&colliderTransform_, color_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 球のコライダー追加
	AddColliderSphere("Boss", &worldPos_, &colliderRadius_);

}

void Enemy::Update()
{
	worldPos_ = transform_.GetWorldPos();

	colliderTransform_ = transform_;
	colliderTransform_.translate_ = worldPos_;
	colliderTransform_.scale_ = { colliderRadius_, colliderRadius_, colliderRadius_};
}

void Enemy::DisplayImGui()
{
	transform_.DisplayImGui();

	bodyTransform_.DisplayImGuiWithTreeNode("BodyTransform");
	headTransform_.DisplayImGuiWithTreeNode("HeadTransform");
	armTransform_R_.DisplayImGuiWithTreeNode("Arm_R_Transform");
	armTransform_L_.DisplayImGuiWithTreeNode("Arm_L_Transform");
}

void Enemy::OnCollisionEnter(Collider* collider)
{
	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	}
}

void Enemy::OnCollision(Collider* collider)
{
	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		//color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
	}
}

void Enemy::OnCollisionExit(Collider* collider)
{
	// 剣と衝突していたら
	if (collider->GetColliderName() == "Sword") {
		color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	}
}
