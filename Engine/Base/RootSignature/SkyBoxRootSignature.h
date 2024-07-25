#pragma once
#include "IRootSignature.h"

/// <summary>
/// スカイボックス用ルートシグネチャ
/// </summary>
class SkyBoxRootSignature : public IRootSignature
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;
};

