#pragma once
#include "IRenderer.h"
#include "../Resource/Rendering/BackBuffer.h"
#include "../Resource/Rendering/DepthStencil.h"
#include "../DXC/DXC.h"

/// <summary>
/// 通常描画用レンダラークラス
/// </summary>
class NormalRenderer
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	NormalRenderer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~NormalRenderer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="signature">ルートシグネチャ</param>
	/// <param name="dxc">DirectXシェーダーコンパイラ</param>
	void Init(DirectXDevice* device, ID3D12RootSignature* signature, DXC* dxc);

	/// <summary>
	/// 描画命令関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	void DrawCall(ID3D12GraphicsCommandList6* list);

private: // メンバ変数

	// パイプラインステートオブジェクト
	PSO pso_;

	// バックバッファ
	BackBuffer* backBuffer_;

	// DSVリソース
	DepthStencil* depthBuffer_;

};

