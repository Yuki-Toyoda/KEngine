#include "Game.h"

void Game::Init()
{
	// 基底クラスの初期化を呼び出す
	KEngineFrameWork::Init();

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
	// 全オブジェクトを削除
	gameObjectManager_->Init();
	// 読み込みスプライト削除
	spriteManager_->Init();
	// 全アニメーション削除
	animManager_->Init();

	// 基底クラスの終了後処理を呼び出す
	KEngineFrameWork::Finalize();
}
