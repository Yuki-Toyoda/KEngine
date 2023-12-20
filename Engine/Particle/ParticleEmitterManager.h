#pragma once
#include "IParticleEmitter.h"

/// <summary>
/// パーティクルエミッタ全てを管理するマネージャ
/// </summary>
class ParticleEmitterManager
{
private: // コンストラクタ、デストラクタ等

	// シングルトンパターンの設定
	ParticleEmitterManager() = default;
	~ParticleEmitterManager() = default;
	ParticleEmitterManager(const ParticleEmitterManager&) = delete;
	const ParticleEmitterManager& operator=(const ParticleEmitterManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ParticleEmitterManager* GetInstance() {
		static ParticleEmitterManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

	/// <summary>
	/// エミッタ生成関数
	/// </summary>
	/// <typeparam name="SelectEmitter">生成するエミッタの型</typeparam>
	/// <typeparam name="SelectParticle">エミッタで生成する粒子の型</typeparam>
	/// <param name="name">生成するエミッタの名前</param>
	/// <param name="maxCount">生成する粒子の最大数</param>
	/// <param name="maxGenerateCount">一度に生成する粒子の数</param>
	/// <param name="translate">エミッタの初期座標</param>
	/// <param name="aliveTime">エミッタの生存時間</param>
	/// <param name="frequency">粒子の生成間隔</param>
	/// <param name="texture">粒子のテクスチャ</param>
	template<IsIParticleEmitter SelectEmitter, IsIParticle SelectParticle>
	inline void CreateEmitter(const std::string& name, int32_t maxCount, int32_t maxGenerateCount, const Vector3& translate, float aliveTime, float frequency, Texture* texture);

private: // メンバ変数

	// エミッタリスト
	std::list<std::unique_ptr<IParticleEmitter>> emitters_;

};

template<IsIParticleEmitter SelectEmitter, IsIParticle SelectParticle>
inline void ParticleEmitterManager::CreateEmitter(const std::string& name, int32_t maxCount, int32_t maxGenerateCount, const Vector3& translate, float aliveTime, float frequency, Texture* texture)
{
	// 新しいエミッタを生成
	std::unique_ptr<SelectEmitter> newEmitter = std::make_unique<SelectEmitter>();
	// エミッタで生成する粒子の型を指定
	newEmitter->SetParticleType<SelectParticle>();
	// 生成したエミッタを初期化
	newEmitter->PreInit(name, maxCount, maxGenerateCount, translate, aliveTime, frequency, texture);
	// 生成したエミッタをリストに追加する
	emitters_.push_back(std::move(newEmitter));
}
