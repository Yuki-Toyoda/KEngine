#pragma once
#include "IRootSignature.h"

/// <summary>
/// モデル用ルートシグネチャ
/// </summary>
class ModelRootSignature : public IRootSignature
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;
};

