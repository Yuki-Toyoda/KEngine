#include "LerpParticle.h"

void LerpParticle::Init()
{

}

void LerpParticle::Update()
{
	//transform_.translate_ = Math::EaseOut()
}

void LerpParticle::SetInfo(const Vector3& start, const Vector3& end, const float& time)
{
	// 各種情報をセット
	start_ = start;
	end_ = end;
	time_ = time;
	// 線形補間タイマー開始
	timer_.Start(time_);
}
