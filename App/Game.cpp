#include "Game.h"

void Game::Init()
{
	// 基底クラスの初期化を呼び出す
	KEngineFrameWork::Init();

	// Srandセット
	KLib::SetSrand();

	// シーンマネージャーにファクトリークラスをセット
	sceneManager_->SetSceneFactory(std::make_unique<SceneFactory>());
}

void Game::Update()
{
	// 基底クラスの更新を呼び出す
	KEngineFrameWork::Update();
}

void Game::Draw()
{
	// 描画
	dxCommon_->Draw();
}

void Game::Finalize()
{
	// 基底クラスの終了後処理を呼び出す
	KEngineFrameWork::Finalize();
}
