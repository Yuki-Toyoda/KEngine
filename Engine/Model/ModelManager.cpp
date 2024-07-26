#include "ModelManager.h"

void ModelManager::Init()
{
	// モデルデータマネージャーの取得
	modelDataManager_ = ModelDataManager::GetInstance();

	// モデル配列の内容クリア
	normalModels_.clear();
	skiningModels_.clear();
	spriteModels_.clear();
	particleModels_.clear();
}

void ModelManager::Clear()
{
	// モデル配列の内容クリア
	normalModels_.clear();
	skiningModels_.clear();
	spriteModels_.clear();
	particleModels_.clear();
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

	// 破壊フラグの立ったスプライトモデルを削除
	spriteModels_.remove_if([](std::unique_ptr<SpriteModel>& model) {
		if (model->isDestroy_) {
			return true;
		}
		return false;
	});

	// 破壊フラグの立ったスプライトモデルを削除
	particleModels_.remove_if([](std::unique_ptr<ParticleModel>& model) {
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

	// 全スプライトモデル更新
	for (std::unique_ptr<SpriteModel>& model : spriteModels_) { // スプライトモデル
		model->Update();
	}

	// 全パーティクルモデル更新
	for (std::unique_ptr<ParticleModel>& model : particleModels_) { // パーティクルモデル
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
		if (model->isActive_){
			model->Draw(cmdList);
		}		
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

void ModelManager::ExecuteComputeSkiningModel(ID3D12GraphicsCommandList6* cmdList)
{
	// 全スキニングモデルのスキニング計算を開始
	for (std::unique_ptr<SkiningModel>& model : skiningModels_) {
		if (model->isActive_) {
			model->ExecuteComputeSkining(cmdList);
		}
	}
}

void ModelManager::SkiningModelDraw(ID3D12GraphicsCommandList6* cmdList)
{
	// 全スキニングモデル描画
	for (std::unique_ptr<SkiningModel>& model : skiningModels_) {
		if (model->isActive_) {
			model->Draw(cmdList);
		}
	}
}

SpriteModel* ModelManager::CreateSpriteModel(ModelData* modelData)
{
	// 新規モデル生成
	std::unique_ptr<SpriteModel> newModel = std::make_unique<SpriteModel>(); // 生成
	newModel->Init(modelData);												 // 初期化

	// インスタンス返還用のモデルを取得
	SpriteModel* returnModel = newModel.get();

	// 生成したモデルをリストに追加
	spriteModels_.push_back(std::move(newModel));

	// 生成したモデルを返す
	return returnModel;
}

void ModelManager::SpriteModelDraw(ID3D12GraphicsCommandList6* cmdList)
{
	// 全スプライトモデル描画
	for (std::unique_ptr<SpriteModel>& model : spriteModels_) {
		if (model->isActive_) {
			model->Draw(cmdList);
		}
	}
}

ParticleModel* ModelManager::CreateParticleModel(ModelData* modelData, int32_t maxDrawCount)
{
	// 新規モデル生成
	std::unique_ptr<ParticleModel> newModel = std::make_unique<ParticleModel>(); // 生成
	newModel->Init(modelData);													 // 初期化
	newModel->CreateBuffer(maxDrawCount);										 // バッファ生成

	// インスタンス返還用のモデルを取得
	ParticleModel* returnModel = newModel.get();

	// 生成したモデルをリストに追加
	particleModels_.push_back(std::move(newModel));

	// 生成したモデルを返す
	return returnModel;
}

void ModelManager::ParticleModelDraw(ID3D12GraphicsCommandList6* cmdList)
{
	// 全パーティクルモデル描画
	for (std::unique_ptr<ParticleModel>& model : particleModels_) {
		if (model->isActive_) {
			model->Draw(cmdList);
		}
	}
}
