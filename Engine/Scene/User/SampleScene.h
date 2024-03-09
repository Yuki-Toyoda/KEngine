#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"
#include "../../Primitive/3d/Model/Model.h"

/// <summary>
/// サンプルのシーン
/// </summary>
class SampleScene final : public BaseScene {
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // メンバ変数

};

