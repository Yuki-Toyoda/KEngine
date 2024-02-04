#include "MoveParticle.h"

void MoveParticle::Init()
{
	Vector3 randomRotate
		= { Math::RandomF(0,0.5f,3),Math::RandomF(0,0.5f,3) ,0 };

	transform_.rotate_ = randomRotate;
}

void MoveParticle::Update()
{

	// 透明度を補間で下げる
	color_.w = KLib::Lerp(1.0f, 0.0f, KLib::EaseOutQuad(aliveTimer_.GetProgress()));

}
