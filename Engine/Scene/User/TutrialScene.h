#pragma once
#include "../BaseScene.h"
class TutrialScene:public BaseScene
{
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

	// 音再生クラス
	Audio* audio_ = nullptr;

	// BGM
	int32_t bgmHandle_ = 0u;
	int bgmVoiceHadle_ = -1;

	// プレイヤー
	Player* player_ = nullptr;

	// カメラ
	InGameCamera* camera_ = nullptr;

	// ボス
	Boss* boss_ = nullptr;

	// ウリボー
	Uribo* uribo_ = nullptr;
};

