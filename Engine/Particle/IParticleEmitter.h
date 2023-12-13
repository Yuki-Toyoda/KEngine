#pragma once
#include "../Primitive/PrimitiveManager.h"
#include "../Utility/KLib.h"

/// <summary>
/// パーティクル発生源基底クラス
/// </summary>
class IParticleEmitter
{
protected: // サブクラス

	// パーティクル構造体
	struct Particle {
		WorldTransform transform;
		Vector3 velocity;
		Vector4 color;
		bool isActive;
		BasePrimitive* primitive_;
		KLib::DeltaTimer aliveTimer_;
	};

public: // メンバ関数

	/// <summary>
	/// 共通初期化関数
	/// </summary>
	/// <param name="maxCount">粒子最大数</param>
	/// <param name="emitterAliveTime">エミッタ生存時間</param>
	/// <param name="frequency">生成間隔設定</param>
	void PreInit(int32_t maxCount, const float& emitterAliveTime, const float& frequency);

	/// <summary>
	/// 初期化関数
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	virtual void Update();

protected: // メンバ変数

	// 発生座標
	WorldTransform emitTransform_;
	// パーティクルの最大発生数
	int32_t maxEmitCount_;

	// エミッタ自体の生存時間
	float emitterAliveTime_ = 1.0f;
	KLib::DeltaTimer emitterAliveTimer_;
	// 発生頻度
	float frequency_ = 0.5f;
	KLib::DeltaTimer frequencyTimer_;

	// 発生をランダムにするための値
	float randomRange_ = 0.0f;

	// パーティクル再生トリガー
	bool isPlay_;

	// パーティクルごとのデータ達
	std::vector<Particle>particles_;

};

