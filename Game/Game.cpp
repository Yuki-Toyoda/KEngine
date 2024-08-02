#include "Game.h"

void Game::Init()
{
	// 基底クラスの初期化を呼び出す
	KEngineFrameWork::Init();

	//// アニメーションマネージャの初期化
	animManager_ = AnimationManager::GetInstance();
	animManager_->Init();

	// オブジェクトマネージャーの初期化
	gameObjectManager_ = GameObjectManager::GetInstance();
	gameObjectManager_->Init();

	// スプライトマネージャの初期化
	spriteManager_ = SpriteManager::GetInstance();
	spriteManager_->Init();

	// パーティクルマネージャの初期化
	particleManager_ = CPUParticleEmitterManager::GetInstance();
	particleManager_->Init();

	// シーンマネージャーにファクトリークラスをセット
	sceneManager_->SetSceneFactory(std::make_unique<SceneFactory>());

}

void Game::Update()
{
	// 基底クラスの更新を呼び出す
	KEngineFrameWork::Update();

	// オブジェクトマネージャー更新
	gameObjectManager_->Update();

	// スプライトマネージャ更新
	spriteManager_->Update();

	// パーティクルマネージャ更新
	particleManager_->Update();
	particleManager_->DisplayImGui();

	// アニメーションマネージャ更新
	animManager_->Update();
	animManager_->DisplayImGui();
}

void Game::Draw()
{
	// 描画
	dxCommon_->Draw();
}

void Game::Finalize()
{
	// 全オブジェクトを削除
	gameObjectManager_->Init();
	// 読み込みスプライト削除
	spriteManager_->Init();
	// 全アニメーション削除
	animManager_->Init();

	// 基底クラスの終了後処理を呼び出す
	KEngineFrameWork::Finalize();
}
