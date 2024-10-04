#include "SceneFactory.h"

BaseScene* SceneFactory::CreateScene(const std::string& name)
{
	// 新しいシーンの生成
	BaseScene* newScene = new SampleScene();

	// タイトルシーンの生成
	if (name == "Title") {
		// タイトルシーンを生成
		newScene = new TitleScene();
	}

	// ゲームシーンの生成
	if (name == "Game") {
		// ゲームシーンを生成
		newScene = new GameScene();
	}

	// 生成したシーンを受け取り元に渡す
	return newScene;
}
