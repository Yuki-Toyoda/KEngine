#include "BoxBreakParticle.h"

void BoxBreakParticle::Init()
{
	//rotate_ = Math::RandomF(0, 180.0f, 3);
}

void BoxBreakParticle::Update()
{
	// 粒子が移動させる
	transform_.translate_ = transform_.translate_ + velocity_;

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(0.3f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));

	// 粒子の移動ベクトルを徐々に下げる
	float decrementValue = 0.03f;
	velocity_.y -= decrementValue;
}
