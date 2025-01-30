#pragma once
#include "Engine/Scene/BaseScene.h"
#include "App/Scene/SceneList.h"
#include "Engine/GameObject/SampleList.h"

/// <summary>
/// ゲームシーンクラス
/// </summary>
class GameScene final : public BaseScene {
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

	// レベル
	Level* level_ = nullptr;

	// 敵
	Enemy* enemy_ = nullptr;
	// プレイヤー
	Player* player_ = nullptr;

	// ゲームマネージャー
	GameManager* gameManager_ = nullptr;

};

