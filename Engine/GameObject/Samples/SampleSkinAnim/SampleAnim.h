#pragma once
#include "../../IObject.h"

/// <summary>
/// アニメーションのサンプルオブジェクト
/// </summary>
class SampleAnim : public IObject
{
	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

	/// <summary>
	/// 更新関数
	/// </summary>
	void Update() override;

private: // プライベートなメンバ変数

};

