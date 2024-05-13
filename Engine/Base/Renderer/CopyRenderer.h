#pragma once
#include "IRenderer.h"
#include "../Resource/Rendering/RenderResource.h"
#include "../DXC/DXC.h"

/// <summary>
/// レンダーリソースのコピーを行うレンダラー
/// </summary>
class CopyRenderer : public IRenderer
{
public: // サブクラス

	// 描画ターゲット構造体
	struct Target {
		RenderResource* src; // BackBuffer or RenderResource
		RenderResource* dst; // BackBuffer or RenderResource
	};

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	CopyRenderer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~CopyRenderer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

	/// <summary>
	/// コピー実行関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Execute(ID3D12GraphicsCommandList6* cmdList);
};

