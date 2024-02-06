#include "BoxBreakParticle.h"

void BoxBreakParticle::Init()
{
}

void BoxBreakParticle::Update()
{
	// 粒子が移動させる
	transform_.translate_ = transform_.translate_ + velocity_;

	transform_.rotate_.x += 0.1f;

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));

	// 粒子の移動ベクトルを徐々に下げる
	velocity_.y -= 0.015f;
}
