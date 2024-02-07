#pragma once
#include "../IParticle.h"

class HealParticle : public IParticle
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

	// 線形補間始端座標
	Vector3 startPos_ = { 0.0f, 0.0f, 0.0f };
	// 線形補間終端座標
	Vector3 endPos_ = { 0.0f, 0.0f, 0.0f };

	// 線形補間始端スケール
	Vector3 startScale_ = { 1.0f, 1.0f, 1.0f };

};

