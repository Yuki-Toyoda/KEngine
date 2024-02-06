#pragma once
#include "../IParticle.h"

class BoxBreakParticle : public IParticle
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

	// 開始時ベクトル格納用
	Vector3 startVelocity_ = { 0.0f, 0.0f, 0.0f };

};

