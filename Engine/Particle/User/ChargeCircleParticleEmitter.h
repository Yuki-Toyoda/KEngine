#pragma once
#include "../IParticleEmitter.h"

class ChargeCircleParticleEmitter : public IParticleEmitter
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 粒子生成関数
	/// </summary>
	void GenerateParticle() override;


};
