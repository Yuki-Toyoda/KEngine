#pragma once
#include "../IParticle.h"

class ChargeCircleParticle : public IParticle
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

	// 線形補間始端スケール
	Vector3 startScale_ = { 1.0f, 1.0f, 1.0f };

	Vector4 startColor_ = {};

};

