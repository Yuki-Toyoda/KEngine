#include "BaseScene.h"
#include "Engine/Sprite/SpriteManager.h"
#include "Engine/Particle/CPU/CPUParticleEmitterManager.h"
#include "Engine/Particle/ParticleManager.h"

void BaseScene::PreInitialize()
{
	// 入力検知
	input_ = Input::GetInstance();
	// ゲームオブジェクトマネージャーのインスタンス取得
	gameObjectManager_ = GameObjectManager::GetInstance();
	// モデルデータマネージャーのインスタンス取得
	modelDataManager_ = ModelDataManager::GetInstance();
	// モデルマネージャーのインスタンス取得
	modelManager_ = ModelManager::GetInstance();

	// 全ゲームオブジェクト削除
	gameObjectManager_->Init();
	// 読み込み済みモデルクリア
	modelManager_->Clear();
	// 生成済みスプライトのクリア
	SpriteManager::GetInstance()->Init();
	// 生成済みCPUパーティクルのクリア
	CPUParticleEmitterManager::GetInstance()->Init();
	// 生成済みGPUパーティクルのクリア
	ParticleManager::GetInstance()->Clear();

	// カメラを生成
	camera_ = gameObjectManager_->CreateInstance<Camera>("DebugCamera", IObject::TagCamera);
	// このカメラを使う
	camera_->UseThisCamera();

	// スカイボックス生成
	skyBox_ = std::make_unique<SkyBox>();

	// サンプルモデルの読み込み
	SystemModelLoad();
}

void BaseScene::DisplayImGui()
{
	// ImGui開始
	ImGui::Begin("Scene");

	// スカイボックスのImGuiを表示
	skyBox_->DisplayImGui("SkyBox");

	// ImGui終了
	ImGui::End();
}

void BaseScene::SystemModelLoad()
{
	// 必須モデルの読み込みをここで行う
	modelDataManager_->LoadNewModel("./Engine/Resource/System/Plane", "Plane.obj");
}
