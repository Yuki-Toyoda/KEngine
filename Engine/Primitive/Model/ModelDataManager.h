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

public: // メンバ変数

	
private: // メンバ変数

	// モデル情報格納用配列
	std::map<std::string, ModelData> modelMap_;

};

