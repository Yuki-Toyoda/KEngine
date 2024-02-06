#include "MoveParticle.h"

void MoveParticle::Init()
{
	//rotate_ = Math::RandomF(0.0f, 1.57f, 2);
}

void MoveParticle::Update()
{

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(0.3f, 0.0f, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

}
