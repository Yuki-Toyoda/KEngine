#pragma once
#include "../../IObject.h"

/// <summary>
/// パーティクルのサンプルクラス
/// </summary>
class SampleParticle : public IObject
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

private: // プライベートなメンバ変数

};

