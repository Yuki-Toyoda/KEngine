#include "MoveParticle.h"

void MoveParticle::Init()
{
}

void MoveParticle::Update()
{

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

}
