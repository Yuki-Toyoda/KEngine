#pragma once
#include "../IParticle.h"

/// <summary>
/// 線形補間で移動する粒子
/// </summary>
class LerpParticle : public IParticle
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

	/// <summary>
	/// 情報セッター
	/// </summary>
	/// <param name="start">始端座標</param>
	/// <param name="end">終端座標</param>
	/// <param name="time">補間にかかる秒数</param>
	void SetInfo(const Vector3& start, const Vector3& end, const float& time);

private: // メンバ変数

	// 始端座標
	Vector3 start_;
	// 終端座標
	Vector3 end_;

	// 線形補間タイマー
	KLib::DeltaTimer timer_;
	// 補間秒数
	float time_;

};

