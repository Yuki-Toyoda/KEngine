#pragma once
#include "Engine/GameObject/IObject.h"

// クラスの前方宣言
class Player;
class Enemy;

/// <summary>
/// ゲームシーンのマネージャー
/// </summary>
class GameManager : IObject
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

	// ImGui表示関数
	// ImGuiを表示させたい場合はこの関数に処理を追記
	void DisplayImGui() override;

private: // メンバ変数

	// ゲーム開始フラグ
	bool isStartGame_ = false;
	// ゲーム終了フラグ
	bool isEndGame_ = false;

	// フェード管理フラグ
	bool isFadeIn_ = false;
	// フェード演出用タイマー
	KLib::DeltaTimer fadeTimer_;
};

