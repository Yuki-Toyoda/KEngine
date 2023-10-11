#pragma once
#include <map>
#include <vector>
#include <string>
#include <fstream>
#include <istream>

#include "../Base/DirectXCommon.h"
#include "../math/Math.h"
#include "OBJ.h"

/// <summary>
/// 読み込むモデル全てを管理するマネージャ
/// </summary>
class ModelManager
{
private: // コンストラクタ等

	// コピーコンストラクタ、代入演算子無効化
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	ModelManager& operator=(const ModelManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得関数
	/// </summary>
	/// <returns>インスタンス</returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Initialize();

	/// <summary>
	/// モデルのロードを行う関数
	/// </summary>
	/// <param name="directoryPath">モデルまでのファイルパス</param>
	/// <param name="fileName">ファイル名</param>
	OBJ::ModelData LoadModel(const std::string& directoryPath, const std::string& fileName);
	/// <summary>
	/// .mtlファイル(テクスチャ)のロード関数
	/// </summary>
	/// <param name="directoryPath">mtlファイルまでのファイルパス</param>
	/// <param name="fileName">ファイル名</param>
	OBJ::MaterialData LoadMaterial(const std::string& directoryPath, const std::string& fileName);

private: // メンバ変数

	// モデルデータを格納するコンテナ
	std::map<std::string, OBJ::ModelData> modelDatas_;

};

