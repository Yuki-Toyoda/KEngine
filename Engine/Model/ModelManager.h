#pragma once
#include "Data/ModelDataManager.h"
#include "NormalModel.h"
#include "SkiningModel.h"
#include "SpriteModel.h"
#include "ParticleModel.h"
#include <list>

/// <summary>
/// モデルマネージャー
/// </summary>
class ModelManager
{
private: // コンストラクタ等

	// シングルトンパターンの設定
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ModelManager* GetInstance() {
		// クラスのインスタンスを取得
		static ModelManager instance;
		// インスタンスを返す
		return &instance;
	}

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// リストクリア関数
	/// </summary>
	void Clear();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

public: // 通常モデル用関数群

	/// <summary>
	/// 通常モデル生成関数
	/// </summary>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="fileName">モデル名</param>
	/// <returns>通常モデル</returns>
	NormalModel* CreateNormalModel(const std::string& filePath, const std::string& fileName);

	/// <summary>
	/// 通常モデル描画関数
	/// </summary>
	/// <param name="cmdList">描画に使用するコマンドリスト</param>
	void NormalModelDraw(ID3D12GraphicsCommandList6* cmdList);

private: // 通常モデル用変数群

	// 通常モデル配列
	std::list<std::unique_ptr<NormalModel>> normalModels_;

public: // スキニングモデル用関数群

	/// <summary>
	/// スキニングモデル生成関数
	/// </summary>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="fileName">モデル名</param>
	/// <returns>スキニングモデル</returns>
	SkiningModel* CreateSkiningModel(const std::string& filePath, const std::string& fileName);

	/// <summary>
	/// スキニング計算実行関数
	/// </summary>
	/// <param name="cmdList">計算シェーダーを実行するコマンドリスト</param>
	void ExecuteComputeSkiningModel(ID3D12GraphicsCommandList6* cmdList);

	/// <summary>
	/// スキニングモデル描画関数
	/// </summary>
	/// <param name="cmdList">描画に使用するコマンドリスト</param>
	void SkiningModelDraw(ID3D12GraphicsCommandList6* cmdList);

private: // スキニングモデル用変数群

	// スキニングモデル配列
	std::list<std::unique_ptr<SkiningModel>> skiningModels_;
	
public: // スプライトモデル用関数群

	/// <summary>
	/// スプライトモデル生成関数
	/// </summary>
	/// <param name="modelData">スプライトに使用するモデル</param>
	/// <returns>スプライトモデル</returns>
	SpriteModel* CreateSpriteModel(ModelData* modelData);

	/// <summary>
	/// スプライトモデル描画関数
	/// </summary>
	/// <param name="cmdList">描画に使用するコマンドリスト</param>
	void SpriteModelDraw(ID3D12GraphicsCommandList6* cmdList);

private: // スプライトモデル用変数群

	// スプライトモデル配列
	std::list<std::unique_ptr<SpriteModel>> spriteModels_;

public: // パーティクルモデル用関数群

	
	/// <summary>
	/// パーティクルモデル生成関数
	/// </summary>
	/// <param name="modelData">パーティクルに使用するモデル(マルチマテリアル不可)</param>
	/// <param name="maxDrawCount">最大数</param>
	/// <returns>パーティクルモデル</returns>
	ParticleModel* CreateParticleModel(ModelData* modelData, int32_t maxDrawCount);

	/// <summary>
	/// パーティクルモデル描画関数
	/// </summary>
	/// <param name="cmdList">描画に使用するコマンドリスト</param>
	void ParticleModelDraw(ID3D12GraphicsCommandList6* cmdList);

private: // スキニングモデル用変数群

	// パーティクルモデル配列
	std::list<std::unique_ptr<ParticleModel>> particleModels_;

private: // メンバ変数

	// モデルデータマネージャー
	ModelDataManager* modelDataManager_ = nullptr;

};

