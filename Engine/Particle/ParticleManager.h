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
	/// <returns>生成済み新規パーティクル</returns>
	Particle* CreateNewParticle(const std::string& name);

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
	std::vector<std::unique_ptr<Particle>> particles_;
};