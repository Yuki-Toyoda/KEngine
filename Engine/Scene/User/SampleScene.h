#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"

/// <summary>
/// サンプルオブジェクトのシーン
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

	// オブジェクトの生成座標
	WorldTransform imGuiGenerateTransform_;

};

