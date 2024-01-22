#pragma once
#include "IPlayerState.h"

/// <summary>
/// 待機時行動状態
/// </summary>
class RootState:public IPlayerState
{
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

public:

	//攻撃可能になるための破片最低数
	int kMinCount = 4;

};

