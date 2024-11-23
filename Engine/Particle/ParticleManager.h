#pragma once
#include "Particle.h"

#include <string>
#include <map>
#include <vector>

/// <summary>
/// GPUパーティクルマネージャー
/// </summary>
class ParticleManager
{
	/// <summary>
	/// フレーム時間計算用
	/// </summary>
	struct PerFrame {
		float time;
		float deltaTime;
	};

private: // コンストラクタ等

	// シングルトンパターンの設定
	ParticleManager() = default;
	~ParticleManager() = default;
	ParticleManager(const ParticleManager&) = delete;
	const ParticleManager& operator=(const ParticleManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ParticleManager* GetInstance() {
		// クラスのインスタンスを取得
		static ParticleManager instance;
		// インスタンスを返す
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	ParticleManager& Init();

	/// <summary>
	/// リストクリア関数
	/// </summary>
	void Clear();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// 描画関数
	/// </summary>
	void Draw();

public: // 生成用関数群

	/// <summary>
	/// パーティクルに使用するPSOの生成を行う関数
	/// </summary>
	/// <param name="name">パーティクル名</param>
	/// /// <param name="initShaderPass">初期化に使用するシェーダーまでのパス</param>
	/// <param name="emitShaderPass">生成に使用するシェーダーまでのパス</param>
	/// <param name="updateShaderPass">更新に使用するシェーダーまでのパス</param>
	/// <returns></returns>
	ParticleManager& CreateParticlePSO(
		const std::string& name, 
		const std::string& initShaderPass,
		const std::string& emitShaderPass,
		const std::string& updateShaderPass);

	/// <summary>
	/// 新規パーティクル生成関数
	/// </summary>
	/// <param name="name">生成するパーティクル名</param>
	/// <param name="filePath">パーティクルに使用するモデルまでのファイルパス</param>
	/// <param name="fileName">モデルのファイル名</param>
	/// <param name="lifeTime">パーティクルの生存時間</param>
	/// <param name="isEndless">終了させるまで放出し続けるか</param>
	/// <param name="maxCount">粒子数</param>
	/// <param name="enableLighting">ライティング有効トリガー</param>
	/// <returns>生成済み新規パーティクル</returns>
	Particle* CreateNewParticle(const std::string& name, const std::string& filePath, const std::string& fileName, const float lifeTime, const bool isEndless = false, const int maxCount = 1024, const bool enableLighting = false);

private: // 機能関数群

private: // メンバ変数

	// デバイス
	DirectXDevice* device_ = nullptr;
	// DXC
	DXC* dxc_ = nullptr;
	// SRV
	SRV* srv_ = nullptr;

	// コマンドリスト
	ID3D12GraphicsCommandList6* cmdList_ = nullptr;

	// ルートシグネチャ
	IRootSignature root_{};

	// PSO配列
	std::map<std::string, ParticlePSO> psos_;

	// パーティクル配列
	std::list<std::unique_ptr<Particle>> particles_;

	// パーティクル計算シェーダーデフォルトパス
	std::string defaultDirectory_ = "Engine/Resource/Shader/Particle/user/";

	// フレーム時間計測バッファ
	std::unique_ptr<ConstantBuffer<PerFrame>> perFrameDataBuffer_;
};