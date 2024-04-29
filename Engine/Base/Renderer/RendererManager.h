#pragma once
#include "../Command.h"
#include "../RootSignature/RootSignatureManager.h"
#include "NormalRenderer.h"

// クラスの前方宣言
class Camera;

/// <summary>
/// レンダラー管理用マネージャー
/// </summary>
class RendererManager
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	RendererManager() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~RendererManager() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="srv">SRV</param>
	void Init(DirectXDevice* device, SRV* srv);

	/// <summary>
	/// 描画開始関数
	/// </summary>
	void DrawCall();

public: // アクセッサ等

	/// <summary>
	/// 描画ターゲット追加関数
	/// </summary>
	/// <param name="view">カメラデータまでのアドレス</param>
	/// <param name="backBuffer">BackBufferリソース</param>
	/// <param name="depth">DSVバッファリソース</param>
	void SetTarget(const D3D12_GPU_VIRTUAL_ADDRESS& view, BackBuffer* backBuffer, DepthStencil* depthBuffer) { normalRenderer_.SetTarget({ view, backBuffer, depthBuffer }); }

	/// <summary>
	/// コマンド管理クラスゲッター
	/// </summary>
	/// <returns>コマンド管理クラス</returns>
	Command* GetCommand() { return &command_; }

private: // メンバ変数

	// コマンド管理用
	Command command_;

	// DirectXシェーダーコンパイラ
	DXC dxc_;

	// SRV
	SRV* srv_;

	// 通常描画レンダラー
	NormalRenderer normalRenderer_;

};

