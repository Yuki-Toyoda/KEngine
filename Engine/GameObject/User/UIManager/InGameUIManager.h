#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"

// クラスの前方宣言
class Boss;
class Uribo;
class Player;

/// <summary>
/// インゲーム内でのUIマネージャ
/// </summary>
class InGameUIManager : public BaseObject
{
private: // サブクラス

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
	/// ImGui表示関数
	/// </summary>
	void DisplayImGui() override;

public: // アクセッサ等

	/// <summary>
	/// ボスのセッター
	/// </summary>
	/// <param name="boss">ボスの実体</param>
	void SetBoss(Boss* boss) { boss_ = boss; }

	/// <summary>
	/// ウリボーのセッター
	/// </summary>
	/// <param name="uribo">ウリボー</param>
	void SetUribo(Uribo* uribo) { uribo_ = uribo; }

	/// <summary>
	/// プレイヤーのセッター
	/// </summary>
	/// <param name="player"></param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数

	// 入力検知用
	Input* input_ = nullptr;
	// コントローラー入力
	XINPUT_STATE joyState_; // 現在フレーム用
	XINPUT_STATE preJoyState_; // 前フレーム用

	// ボス本体
	Boss* boss_ = nullptr;

	// ウリボー本体
	Uribo* uribo_ = nullptr;

	// プレイヤー本体
	Player* player_ = nullptr;

	/// スプライト格納用変数群
	Sprite* bossHPGageSprite_BG_ = nullptr; // ボスのHPゲージ背景
	Sprite* bossHPGageSprite_F_ = nullptr; // ボスのゲージ本体
	Sprite* bossHPGageSprite_Icon_ = nullptr; // ボスのアイコン
	Sprite* playerHPFrame_ = nullptr; // プレイヤーのHPフレーム
	Sprite* playerIcon_ = nullptr; // プレイヤーのHPフレーム
	Sprite* playerVegetableIcon_ = nullptr; // プレイヤーの野菜アイコン
	Sprite* playerVegetableCount1_ = nullptr; // 1桁目
	Sprite* playerVegetableCount2_ = nullptr; // 2桁目
};

