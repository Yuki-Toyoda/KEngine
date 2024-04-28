#pragma once
#include "../Command.h"
#include "../RootSignature/RootSignatureManager.h"
#include "NormalRenderer.h"

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

