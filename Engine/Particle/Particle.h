#pragma once
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Math/Vector4.h"
#include "../Math/Quaternion.h"
#include "../GameObject/WorldTransform.h"
#include "../Model/ParticleModel.h"
#include "../Base/PSO/PSO.h"
#include "../Base/RootSignature/IRootSignature.h"

#include <string>

/// <summary>
/// GPUパーティクル
/// </summary>
class Particle
{
private: // サブクラス

	/// <summary>
	/// パーティクル用PSO構造体
	/// </summary>
	struct ParticlePSO {
		PSO init{};
		PSO emit{};
		PSO update{};
	};

	/// <summary>
	/// 球状エミッタ
	/// </summary>
	struct EmitterSphere {
		Vector3 translate;		// 生成中心座標
		float	radius;			// 生成半径
		int32_t count;			// 一度に生成する粒子数
		float	frequency;		// 生成間隔秒数
		float	frequencyTime;	// 生成間隔現在秒数
		int32_t emit;			// 生成トリガー

	};

	/// <summary>
	/// 共通パーティクルデータ
	/// </summary>
	struct ParticleData {
		Vector3 translate;	 // 位置座標
		Vector3 scale;		 // 大きさ
		Vector3 velocity;	 // 速度ベクトル
		float	currentTime; // 現在時間
		float	lifeTime;	 // 生存時間全体
		Vector4 color;		 // 色
	};

	/// <summary>
	/// パーティクル情報構造体
	/// </summary>
	struct InfoData {
		int32_t instanceCount; // パーティクル生成数
		int32_t isBillboard;   // ビルボードを行うか
	};

public: // コンストラクタ等

	// デフォルトコンストラクタ削除
	Particle() = default;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="maxParticleSize">パーティクル最大数</param>
	Particle(uint32_t maxParticleSize) : kMaxParticleCount_(maxParticleSize) {};

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Particle() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="srv">SRV</param>
	/// <param name="list">コマンドリスト</param>
	/// <param name="pso">PSO配列</param>
	void Init(DirectXDevice* device, SRV* srv, ID3D12GraphicsCommandList6* list, const ParticlePSO& pso);

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // パブリックメンバ変数

	// 描画するモデル
	ParticleModel* model_ = nullptr;

private: // メンバ変数

	// 粒子最大数
	const uint32_t kMaxParticleCount_;

	// エミッタ用バッファ
	std::unique_ptr<ConstantBuffer<EmitterSphere>> emitterDataBuffer_;
	// 情報バッファ
	std::unique_ptr<ConstantBuffer<InfoData>> infoDataBuffer_;
	// パーティクル用バッファ
	std::unique_ptr<RWStructuredBuffer<ParticleData>> particleDataBuffer_;

	// コマンドリスト
	ID3D12GraphicsCommandList6* cmdList_ = nullptr;
	// パーティクル描画に用いるPSO配列
	ParticlePSO pso_{};
};

