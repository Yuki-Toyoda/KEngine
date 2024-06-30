#include "ModelDataManager.h"

void ModelDataManager::Clear()
{
	// モデル情報用配列のクリア
	modelMap_.clear();
}

void ModelDataManager::LoadNewModel(const std::string& filePath, const std::string& fileName)
{
	// モデル情報配列にデータが存在するか
	if (!modelMap_.count(fileName)) { // 存在しない場合
		// 新規読み込みを行う
		//modelMap_[fileName] = std::make_unique<ModelData>();
		modelMap_[fileName]->Load(filePath, fileName);
	}
}

ModelData* ModelDataManager::GetModelData(const std::string& filePath, const std::string& fileName)
{
	// 返還用モデルデータ
	ModelData* returnData = nullptr;

	// モデル情報配列にデータが存在するか
	if (!modelMap_.count(fileName)) { // 存在しない場合
		// 新規読み込みを行う
		LoadNewModel(filePath, fileName);
	}

	// モデルデータを取得
	returnData = modelMap_[fileName].get();
	// 取得したモデルデータを返す
	return returnData;
}
