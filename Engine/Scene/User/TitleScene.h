#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene final : public BaseScene {
public: // メンバ関数

	/// <summary>
	/// モデル読み込み関数
	/// </summary>
	void LoadModel() override;

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

