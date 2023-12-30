#include "SampleSphere.h"

void SampleSphere::Init()
{
	// メッシュを追加
	AddMesh(&transform_, color_, "./Engine/Resource/Samples/Sphere", "Sphere.obj");

	// 球のコライダー追加
	AddColliderSphere("Sphere", &transform_.translate_, &transform_.scale_.x);

}

void SampleSphere::Update()
{

}

void SampleSphere::OnCollisionEnter(Collider* collider)
{
	collider;
	color_ = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void SampleSphere::OnCollisionExit(Collider* collider)
{
	collider;
	color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
}
