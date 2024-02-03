#pragma once
#include "BaseScene.h"
#include <string>

/// <summary>
/// シーンファクトリー基底クラス
/// </summary>
class ISceneFactory {
public: // 仮想デストラクタ

	// 仮想デストラクタ
	virtual ~ISceneFactory() = default;

public: // メンバ変数

	/// <summary>
	/// シーン生成関数
	/// </summary>
	/// <param name="name">生成するシーン名</param>
	virtual BaseScene* CreateScene(const std::string& name) = 0;

};