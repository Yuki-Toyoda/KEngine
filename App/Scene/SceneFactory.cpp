#include "SceneFactory.h"

BaseScene* SceneFactory::CreateScene(const std::string& name)
{
	// 新しいシーンの生成
	BaseScene* newScene = nullptr;

	// ゲームシーンの生成
	if (name == "Game") {
		// ゲームシーンを生成
		newScene = new GameScene();

		// 生成したシーンを返還
		return newScene;
	}

	// ここまで来た場合サンプルシーンを生成
	newScene = new SampleScene();

	// 生成したシーンを受け取り元に渡す
	return newScene;
}
