#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"

/// <summary>
/// タイトルシーンクラス
/// </summary>
class TitleScene final : public BaseScene {
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

	// タイトルマネージャ
	TitleManager* tm_ = nullptr;

};

