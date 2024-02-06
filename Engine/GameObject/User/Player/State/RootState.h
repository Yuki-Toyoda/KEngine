#pragma once
#include "IPlayerState.h"
#include "../../../../Audio/Audio.h"
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

private: // プライベートなメンバ関数

	/// <summary>
	/// 移動の関する処理をまとめた関数
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃に関する処理をまとめた関数
	/// </summary>
	void Attack();

public:

	//攻撃可能になるための破片最低数
	int kMinCount = 1;
	//チャージ
	int32_t charge_ = 0u;
	int32_t chargeEnd_ = 0u;
	int chargeHandle=-1;
	int chargeEndhandle=-1;
	Audio* audio_;
};

