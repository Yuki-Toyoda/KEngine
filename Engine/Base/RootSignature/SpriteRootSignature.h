#pragma once
#include "IRootSignature.h"

/// <summary>
/// スプライト用ルートシグネチャ
/// </summary>
class SpriteRootSignature : public IRootSignature
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;
};

