#pragma once
#include "IState.h"

// クラスの前方宣言
class ComboManager;

/// <summary>
/// 攻撃状態クラス
/// </summary>
class Attack : public IState
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
	/// ImGuiの表示関数
	/// </summary>
	void DisplayImGui() override;

private: // メンバ変数

	// コンボマネージャー本体
	ComboManager* comboManager_ = nullptr;

};

