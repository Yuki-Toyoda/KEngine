#pragma once
#include "../IParticleEmitter.h"

class RubbleFadeParticleEmitter : public IParticleEmitter
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 粒子生成
	/// </summary>
	void GenerateParticle() override;

};

