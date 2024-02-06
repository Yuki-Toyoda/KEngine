#pragma once
#include "../IParticle.h"

class MoveParticle : public IParticle
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

private: // メンバ変数


};

