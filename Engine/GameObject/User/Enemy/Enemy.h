#pragma once
#include "../../BaseObject.h"

/// <summary>
/// 敵クラス
/// </summary>
class Enemy : public BaseObject
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

private: // その他関数

	// 身体のトランスフォーム
	WorldTransform bodyTransform_;



};

