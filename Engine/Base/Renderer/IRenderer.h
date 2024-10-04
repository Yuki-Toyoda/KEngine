#pragma once
#include "Engine/Base/RootSignature/RootSignatureManager.h"
#include "Engine/Base/PSO/PSO.h"
#include "Engine/Base/Resource/Data/StructuredBuffer.h"

/// <summary>
/// 描画レンダラー基底クラス
/// </summary>
class IRenderer
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IRenderer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~IRenderer() = default;

};

