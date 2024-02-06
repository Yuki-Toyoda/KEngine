#include "ChargeCircleParticle.h"

void ChargeCircleParticle::Init()
{
	startScale_ = transform_.scale_;
}

void ChargeCircleParticle::Update()
{
	Vector3 zeroScale = {};
	transform_.scale_ = KLib::Lerp<Vector3>(startScale_, zeroScale, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));
}
