#include "Enemy.h"

void Enemy::Init()
{
	// 各種ワールドトランスフォームの初期化
	bodyTransform_.Init();
	// 親子付け
	bodyTransform_.SetParent(&transform_, 0b011);
	bodyTransform_.translate_ = { 0.0f, 1.0f, 0.0f };

	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 球のコライダー追加
	AddColliderSphere("Sphere", &transform_.translate_, &transform_.scale_.x);

}

void Enemy::Update()
{
}

void Enemy::DisplayImGui()
{

}
