#pragma once
#include "../IPrimitive.h"
#include <map>

/// <summary>
/// メッシュの情報を格納するマネージャー
/// </summary>
class MeshManager
{
private: // コンストラクタ、デストラクタ等

	// シングルトンパターンの設定
	MeshManager() = default;
	~MeshManager() = default;
	MeshManager(const MeshManager&) = delete;
	const MeshManager& operator=(const MeshManager&) = delete;

public: // メンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static MeshManager* GetInstance();

	/// <summary>
	/// メッシュ情報配列内の情報をクリアする関数
	/// </summary>
	void ClearMap();
	
	/// <summary>
	/// メッシュ情報配列内に情報を追加する関数
	/// </summary>
	/// <param name="fullPath">モデルまでのフルパス</param>
	/// <param name="vertices">頂点データ配列</param>
	/// <param name="indexes">インデックスデータ配列</param>
	void AddInfo(std::string fullPath, std::vector<Vertex> vertices, std::vector<uint32_t> indexes);

public: // パブリックメンバ変数

	// 情報格納用マップ
	std::map<std::string, std::vector<Vertex>> meshVertexMap_;  // 頂点
	std::map<std::string, std::vector<uint32_t>> meshIndexMap_;	// インデックスデータ

};

