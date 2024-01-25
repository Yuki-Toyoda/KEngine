#pragma once
#include "../BaseScene.h"
#include "../SceneList.h"
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

private:

	// プレイヤー
	Player*player_;

	// カメラ
	InGameCamera* camera_;

	// ボス
	Boss* boss_;
};
