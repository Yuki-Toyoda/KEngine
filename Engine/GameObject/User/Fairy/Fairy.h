#pragma once
#include "../../IObject.h"

// クラスの前方宣言
class Player;

/// <summary>
/// ナ〇ィ
/// </summary>
class Fairy : public IObject
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

public: // アクセッサ

	/// <summary>
	/// プレイヤーセッター
	/// </summary>
	/// <param name="player">プレイヤー</param>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数

	// プレイヤー
	Player* player_ = nullptr;

};

