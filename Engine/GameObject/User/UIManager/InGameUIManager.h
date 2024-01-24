#pragma once
#include "../../BaseObject.h"
#include "../../../Input/Input.h"

// クラスの前方宣言
class Boss;
class Uribo;

/// <summary>
/// インゲーム内でのUIマネージャ
/// </summary>
class InGameUIManager : public BaseObject
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

};

