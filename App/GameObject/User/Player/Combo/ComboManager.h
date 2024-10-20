#pragma once
#include "Combo.h"

/// <summary>
/// コンボ管理マネージャー
/// </summary>
class ComboManager
{
private: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	ComboManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ComboManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update();

private: // メンバ変数

	// 再生中コンボ
	Combo combo_;

	// コンボ配列
	std::map<std::string, Combo> combos_;

};

