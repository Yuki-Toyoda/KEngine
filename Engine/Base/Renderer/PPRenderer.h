#pragma once
#include "IRenderer.h"
#include "../Resource/Rendering/RenderResource.h"
#include "../Resource/Rendering/DepthStencil.h"
#include "../DXC/DXC.h"

/// <summary>
/// ポストプロセス用レンダラークラス
/// </summary>
class PPRenderer : public IRenderer
{
public: // サブクラス

	// 通常描画のターゲット構造体
	struct Target {
		RenderResource* render;		// ポストプロセスを掛けた結果を格納するリソース
		BackBuffer* texture;		// ポストプロセスを掛けるテクスチャ
		DepthStencil* depth;		// デプスのリソース
	};

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	PPRenderer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~PPRenderer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxc">DirectXシェーダーコンパイラ</param>
	void Init(DirectXDevice* device, DXC* dxc);

	/// <summary>
	/// 描画命令関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	void DrawCall(ID3D12GraphicsCommandList6* list);

public: // アクセッサ等

	/// <summary>
	/// 描画ターゲット追加関数
	/// </summary>
	/// <param name="target">描画ターゲット</param>
	void SetTarget(const Target& target) { target_ = target; }

private: // メンバ変数

	// ルートシグネチャ群
	ID3D12RootSignature* rootSignature_ = nullptr;

	// パイプラインステートオブジェクト
	PSO pso_;

	// 描画ターゲット
	Target target_;

};

