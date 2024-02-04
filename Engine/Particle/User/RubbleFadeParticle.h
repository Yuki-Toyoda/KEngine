#pragma once
#include "../IParticle.h"

class RubbleFadeParticle : public IParticle
{
public: // メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 乱数で設定する部分
	/// </summary>
	void SetUp(float amp, float frequency, int side) {
		amplitude_ = amp;
		frequency_ = frequency;
		moveSide_ = side;
	}

private: // メンバ変数


	Vector3 startScale_ = {};	// 初期サイズ

	float amplitude_;	// 振幅
	float frequency_;	// 振動数
	int moveSide_;		// 動く方向（左右

};

