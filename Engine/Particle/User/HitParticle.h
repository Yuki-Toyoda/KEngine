#pragma once
#include "../IParticle.h"

/// <summary>
/// 命中パーティクル粒子
/// </summary>
class HitParticle : public IParticle
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update();

private: // メンバ変数

	// 開始時ベクトル格納用
	Vector3 startVelocity_ = { 0.0f, 0.0f, 0.0f };

};

