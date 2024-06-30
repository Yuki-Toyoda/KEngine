#pragma once
#include "ModelData.h"
#include <map>

/// <summary>
/// モデルデータマネージャー
/// </summary>
class ModelDataManager
{
private: // コンストラクタ等

	// シングルトンパターンの設定
	ModelDataManager() = default;
	~ModelDataManager() = default;
	ModelDataManager(const ModelDataManager&) = delete;
	const ModelDataManager& operator=(const ModelDataManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ModelDataManager* GetInstance() {
		// クラスのインスタンスを取得
		static ModelDataManager instance;
		// インスタンスを返す
		return &instance;
	}

	/// <summary>
	/// 配列データクリア関数
	/// </summary>
	void Clear();
	
	/// <summary>
	/// 新規モデル読み込み関数
	/// </summary>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="fileName">ファイル名</param>
	void LoadNewModel(const std::string& filePath, const std::string& fileName);

public: // アクセッサ等

	/// <summary>
	/// <para>モデルデータゲッター</para>
	/// <para>モデルデータ配列に無ければ新規読み込みを行うが、初期化以外での読み込みはNG</para>
	/// </summary>
	/// <param name="filePath">モデルまでのファイルパス</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns></returns>
	ModelData* GetModelData(const std::string& filePath, const std::string& fileName);

private: // メンバ変数

	// モデル情報格納用配列
	std::map<std::string, std::unique_ptr<ModelData>> modelMap_;

};

