#include "MeshManager.h"

MeshManager* MeshManager::GetInstance()
{
	static MeshManager instance;
	return &instance;
}

void MeshManager::ClearMap()
{
	// 読み込んでいるメッシュのクリア
	meshVertexMap_.clear();
	meshIndexMap_.clear();
}

void MeshManager::AddInfo(std::string fullPath, std::vector<Vertex> vertices, std::vector<uint32_t> indexes)
{
	// それぞれの情報配列に情報追加
	meshVertexMap_.insert({fullPath, vertices}); // 頂点
	meshIndexMap_.insert({fullPath, indexes});	 // インデックス
}
