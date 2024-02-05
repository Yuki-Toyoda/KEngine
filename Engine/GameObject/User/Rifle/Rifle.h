#pragma once
#include "../../BaseObject.h"
#include "../../../Utility/Animation/AnimationManager.h"
#include "../../../Input/Input.h"
#include "../../../Audio/Audio.h"

/// <summary>
/// ライフルクラス
/// </summary>
class Rifle : public BaseObject
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

	/// <summary>
	/// ImGuiを表示させる関数
	/// </summary>
	void DisplayImGui() override;

public: // その他メンバ関数

	/// <summary>
	/// パラメータ生成関数
	/// </summary>
	/// <param name="name">生成するパラメータ名</param>
	void CreateParameter(const std::string& name);

private: // メンバ変数

	// 入力検知用
	Input* input_;

	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// 音再生クラス
	Audio* audio_ = nullptr;

	// 効果音
	int32_t shotSE_ = 0u; // 射撃
	int32_t cockSE_ = 0u; // コッキング
	int32_t dropSE_ = 0u; // 薬莢
	int32_t reloadSE_ = 0u; // リロード

	// 銃のトランスフォーム
	WorldTransform baseTransform_;
	// フォアエンドのトランスフォーム
	WorldTransform forendTransform_;
	// 弾のトランスフォーム
	WorldTransform bulletTransform_;

	// アニメーションマネージャー
	AnimationManager* animManager_ = nullptr;
	// アニメーション
	Animation* anim_ = nullptr;

	// 弾数
	int32_t ammoCount_ = 8;
	// 最大弾数
	int32_t maxAmmoCount_ = 8;

	// 音再生トリガー
	bool isReload_ = false;
	// 打ち切りリロードトリガー
	bool isNoAmmoReload_ = false;

};