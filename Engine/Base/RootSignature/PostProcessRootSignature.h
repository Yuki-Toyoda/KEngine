#pragma once
#include "IRootSignature.h"

/// <summary>
/// ポストプロセス用ルートシグネチャ
/// </summary>
class PostProcessRootSignature : public IRootSignature
{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init() override;

};

