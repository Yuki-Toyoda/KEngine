#pragma once
#include "Engine/Scene/BaseScene.h"
#include "../SceneList.h"
#include "../../GameObject/SampleList.h"

/// <summary>
/// ゲームシーンクラス
/// </summary>
class GameScene final : public BaseScene {
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

	// レベル
	Level* level_ = nullptr;

	// 敵
	Enemy* enemy_ = nullptr;
	// プレイヤー
	Player* player_ = nullptr;

};

