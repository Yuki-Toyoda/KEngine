#pragma once
#include "../Buffer/BufferStructs.h"
#include "../RootSignature/RootSignatureManager.h"
#include "../PSO/PSO.h"
#include "../Resource/Data/StructuredBuffer.h"

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

