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
#include <DirectXMath.h>

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
	/// 球状エミッタ
	/// </summary>
	__declspec(align(16)) struct EmitterSphere {
		Vector3 translate = {};			// 生成中心座標
		float	radius = 0.0f;			// 生成半径
		float	frequency = 0.5f;		// 生成間隔秒数
		float	frequencyTime = 0.0f;	// 生成間隔現在秒数
		int32_t count = 10;				// 一度に生成する粒子数
		int32_t emit = false;			// 生成トリガー
	};

	/// <summary>
	/// パーティクルデータ
	/// </summary>
	struct ParticleData {
		Vector4 color;		 // 色
		Vector3 translate;	 // 位置座標
		float	currentTime; // 現在時間
		Vector3 scale;		 // 大きさ
		float	lifeTime;	 // 生存時間全体
		Vector3 rotate;		 // 回転角
		float	padding;	 // パディング回避用
		Vector3 velocity;	 // 速度ベクトル
		float	padding2;	 // パディング回避用
	};

	/// <summary>
	/// パーティクル情報構造体
	/// </summary>
	struct InfoData {
		int32_t instanceCount;	// パーティクル生成数
		int32_t isBillboard;	// ビルボードを行うか
		float padding[2];		// パディング回避用
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
	/// <param name="isEndless">終了指示がくるまで出続けるか</param>
	/// <param name="enableLighting">ライティングを有効にするか</param>
	void Init(DirectXDevice* device, SRV* srv, ID3D12GraphicsCommandList6* list, const ParticlePSO& pso, const std::string& filePath, const std::string& fileName, const float lifeTime, const bool isEndless, const bool enableLighting);

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

	/// <summary>
	/// ループ状態ゲッター
	/// </summary>
	/// <returns>ループ状態</returns>
	bool GetIsEndless() { return isEndless_; }

	/// <summary>
	/// 終了状態ゲッター
	/// </summary>
	/// <returns>終了状態</returns>
	bool GetIsEnd() { return isEnd_; }
	/// <summary>
	/// 終了状態セッター
	/// </summary>
	/// <param name="isEnd">設定する終了状態</param>
	void SetIsEnd(const bool isEnd) { isEnd_ = isEnd; }

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

	// パーティクルを無限に出すか
	bool isEndless_ = false;
	// パーティクルの終了フラグ
	bool isEnd_ = false;

	// 粒子最大数
	const uint32_t kMaxParticleCount_;

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

