#include "IParticleEmitter.h"

void IParticleEmitter::PreInit(int32_t maxCount, const float& emitterAliveTime, const float& frequency)
{
	// 最大数設定
	maxEmitCount_ = maxCount;
	// 発生頻度設定
	frequency_ = frequency;
	// 生存時間設定
	emitterAliveTime_ = emitterAliveTime;

	// タイマーに値をセットする
	emitterAliveTimer_.Start(emitterAliveTime_);
	frequencyTimer_.Start(frequency_);
}

void IParticleEmitter::Init()
{

}

void IParticleEmitter::Update()
{
	// 再生中でなければ粒子を動かさない
	if (isPlay_) {
		
	}
}
