#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 命中パーティクル
/// </summary>
class SampleHitParticle : public BaseObject
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

	void SetTarget(const Vector3& translate) { target_ = &translate; }

private: // メンバ変数

	// 追従ターゲット
	const Vector3* target_;

	// 粒子最大数
#ifdef _DEBUG
	static const int kMaxParticle = 15;
#endif // _DEBUG

#ifndef _DEBUG
	static const int kMaxParticle = 50;
#endif // !_DEBUG


	// 粒子の拡散範囲
	const float kMaxSpred = 10.0f;

	// 粒子演出用t
	float t_[kMaxParticle];
	// 粒子ごとの演出時間
	float time_[kMaxParticle];

	// 粒子演出用座標
	Vector3 startTranslate_[kMaxParticle]; // 始端座標
	Vector3 endTranslate_[kMaxParticle];   // 終端座標

	// それぞれの粒子の終了フラグ
	bool isEnd_[kMaxParticle];

	// 粒子ごとのワールドトランスフォーム
	WorldTransform particleTransform_[kMaxParticle];

};

