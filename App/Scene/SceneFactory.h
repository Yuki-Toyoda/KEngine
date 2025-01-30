#pragma once
#include "Engine/Scene/ISceneFactory.h"
#include "SceneList.h"

/// <summary>
/// シーン生成ファクトリー
/// </summary>
class SceneFactory : public ISceneFactory
{
public: // メンバ関数

	/// <summary>
	/// シーン生成関数
	/// </summary>
	/// <param name="name">生成するシーン名</param>
	BaseScene* CreateScene(const std::string& name) override;

};

