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
	Player*player_ = nullptr;

	// カメラ
	InGameCamera* camera_ = nullptr;

	// ボス
	Boss* boss_ = nullptr;

	// ウリボー
	Uribo* uribo_ = nullptr;
};
