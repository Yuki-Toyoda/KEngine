#include "HitParticle.h"

void HitParticle::Init()
{
	decrementValue_ = Math::RandomF(0.015f, 0.025f, 3);
	//decrementValue_ = Math::RandomF(0.025f, 0.03f, 3);
}
void HitParticle::Update()
{
	// 粒子が移動させる
	transform_.translate_ = transform_.translate_ + velocity_;

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseInOutQuad(aliveTimer_.GetProgress()));

	// 粒子の移動ベクトルを徐々に下げる
	velocity_.y -= decrementValue_;
}
