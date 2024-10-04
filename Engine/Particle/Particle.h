#pragma once
#include "Engine/Math/Vector2.h"
#include "Engine/Math/Vector3.h"
#include "Engine/Math/Vector4.h"
#include "Engine/Math/Quaternion.h"
#include "Engine/GameObject/WorldTransform.h"
#include "Engine/Model/ParticleModel.h"
#include "Engine/Base/PSO/PSO.h"
#include "Engine/Base/RootSignature/IRootSignature.h"
#include "Engine/Utility/Timer/DeltaTimer.h"

#include <string>

/// <summary>
/// パーティクル用PSO構造体
/// </summary>
struct ParticlePSO {
	PSO init{};
	PSO emit{};
	PSO update{};
};

/// <summary>
/// GPUパーティクル
/// </summary>
class Particle
{
private: // サブクラス

	/// <summary>
	/// フレーム時間計算用
	/// </summary>
	struct PerFrame {
		float time;
		float deltaTime;
	};

	/// <summary>
	/// 球状エミッタ
	/// </summary>
	struct EmitterSphere {
		Vector3 translate = {};			// 生成中心座標
		float	radius = 0.0f;			// 生成半径
		int32_t count = 10;				// 一度に生成する粒子数
		float	frequency = 0.5f;		// 生成間隔秒数
		float	frequencyTime = 0.0f;	// 生成間隔現在秒数
		int32_t emit = false;			// 生成トリガー

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
	/// <param name="filePath">使用モデルまでのファイルパス</param>
	/// <param name="fileName">使用モデルのファイル名</param>
	/// <param name="lifeTime">パーティクル自体の生存秒数</param>
	/// <param name="enableLighting">ライティングを有効にするか</param>
	void Init(DirectXDevice* device, SRV* srv, ID3D12GraphicsCommandList6* list, const ParticlePSO& pso, const std::string& filePath, const std::string& fileName, const float lifeTime, const bool enableLighting);

	/// <summary>
	/// 初期化シェーダーの実行関数
	/// </summary>
	void ExecuteInit();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // アクセッサ等

	/// <summary>
	/// 初期化状態ゲッター
	/// </summary>
	/// <returns>初期化</returns>
	bool GetIsInit() { return isInit_; }

public: // パブリックメンバ変数

	// パーティクル中心座標
	WorldTransform transform_;

	// 描画するモデル
	ParticleModel* model_ = nullptr;

	// パーティクル自体の生存時間タイマー
	KLib::DeltaTimer timer_{};

	// エミッタ用バッファ
	std::unique_ptr<ConstantBuffer<EmitterSphere>> emitterDataBuffer_;

private: // メンバ変数

	// 初期化フラグ
	bool isInit_ = false;

	// 粒子最大数
	const uint32_t kMaxParticleCount_;

	// フレーム時間計測バッファ
	std::unique_ptr<ConstantBuffer<PerFrame>> perFrameDataBuffer_;
	// 情報バッファ
	std::unique_ptr<ConstantBuffer<InfoData>> infoDataBuffer_;
	// パーティクル用バッファ
	std::unique_ptr<RWStructuredBuffer<ParticleData>> particleBuffer_;
	// 使用可能番号用バッファ
	std::unique_ptr<RWStructuredBuffer<int32_t>> freeIndexBuffer_;
	// 使用可能番号リスト用バッファ
	std::unique_ptr<RWStructuredBuffer<uint32_t>> freeIndexListBuffer_;

	// コマンドリスト
	ID3D12GraphicsCommandList6* cmdList_ = nullptr;
	// パーティクル描画に用いるPSO配列
	ParticlePSO pso_{};

};

