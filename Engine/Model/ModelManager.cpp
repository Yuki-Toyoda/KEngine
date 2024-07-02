#include "ModelManager.h"

void ModelManager::Init()
{
	// モデルデータマネージャーの取得
	modelDataManager_ = ModelDataManager::GetInstance();

	// モデル配列の内容クリア
	normalModels_.clear();
}

void ModelManager::Clear()
{
	// モデル配列の内容クリア
	normalModels_.clear();
}

void ModelManager::Update()
{
	// 破壊フラグの立った通常モデルを削除
	normalModels_.remove_if([](std::unique_ptr<NormalModel>& model) {
		if (model->isDestroy_) {
			return true;
		}
		return false;
	});

	// 破壊フラグの立ったスキニングモデルを削除
	skiningModels_.remove_if([](std::unique_ptr<SkiningModel>& model) {
		if (model->isDestroy_) {
			return true;
		}
		return false;
	});

	// 全通常モデル更新
	for (std::unique_ptr<NormalModel>& model : normalModels_) { // 通常モデル
		model->Update();
	}
	
	// 全スキニングモデル更新
	for (std::unique_ptr<SkiningModel>& model : skiningModels_) { // スキニングモデル
		model->Update();
	}
}

NormalModel* ModelManager::CreateNormalModel(const std::string& filePath, const std::string& fileName)
{
	// 新規モデル生成
	std::unique_ptr<NormalModel> newModel = std::make_unique<NormalModel>(); // 生成
	newModel->Init(modelDataManager_->GetModelData(filePath, fileName));	 // 初期化
	
	// インスタンス返還用のモデルを取得
	NormalModel* returnModel = newModel.get();

	// 生成したモデルをリストに追加
	normalModels_.push_back(std::move(newModel));

	// 生成したモデルを返す
	return returnModel;
}

void ModelManager::NormalModelDraw(ID3D12GraphicsCommandList6* cmdList)
{
	// 全通常モデル描画
	for (std::unique_ptr<NormalModel>& model : normalModels_) {
		model->Draw(cmdList);
	}
}

SkiningModel* ModelManager::CreateSkiningModel(const std::string& filePath, const std::string& fileName)
{
	// 新規モデル生成
	std::unique_ptr<SkiningModel> newModel = std::make_unique<SkiningModel>(); // 生成
	newModel->Init(modelDataManager_->GetModelData(filePath, fileName));	   // 初期化

	// インスタンス返還用のモデルを取得
	SkiningModel* returnModel = newModel.get();

	// 生成したモデルをリストに追加
	skiningModels_.push_back(std::move(newModel));

	// 生成したモデルを返す
	return returnModel;
}

void ModelManager::SkiningModelDraw(ID3D12GraphicsCommandList6* cmdList)
{
	// 全スキニングモデル描画
	for (std::unique_ptr<SkiningModel>& model : skiningModels_) {
		model->Draw(cmdList);
	}
}
