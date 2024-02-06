#include "ChargeCircleParticle.h"

void ChargeCircleParticle::Init()
{
	startScale_ = transform_.scale_;
	startColor_ = color_;
}

void ChargeCircleParticle::Update()
{
	Vector3 zeroScale = {};
	transform_.scale_ = KLib::Lerp<Vector3>(startScale_, zeroScale, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

	// 透明度を補間で下げる
	//color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));

	color_.x = KLib::Lerp(startColor_.x, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));
	color_.y = KLib::Lerp(startColor_.y, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));
	color_.z = KLib::Lerp(startColor_.z, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));
	color_.w = KLib::Lerp(startColor_.w, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));

}
