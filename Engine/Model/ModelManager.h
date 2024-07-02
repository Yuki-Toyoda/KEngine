#pragma once
#include "Data/ModelDataManager.h"
#include "NormalModel.h"
#include "SkiningModel.h"
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
	/// スキニングモデル描画関数
	/// </summary>
	/// <param name="cmdList">描画に使用するコマンドリスト</param>
	void SkiningModelDraw(ID3D12GraphicsCommandList6* cmdList);

private: // スキニングモデル用変数群

	// スキニングモデル配列
	std::list<std::unique_ptr<SkiningModel>> skiningModels_;

private: // メンバ変数

	// モデルデータマネージャー
	ModelDataManager* modelDataManager_ = nullptr;

};

