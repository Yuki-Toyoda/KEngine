#pragma once
#include "../Command.h"
#include "../RootSignature/RootSignatureManager.h"
#include "../../Model/ModelManager.h"
#include "../../Lighting/Light/DirectionalLight.h"
#include "ModelRenderer.h"
#include "PPRenderer.h"
#include "../../PostProcess/PostProcessor.h"
#include "SpriteRenderer.h"

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
	/// DXCゲッター
	/// </summary>
	/// <returns>DXC</returns>
	DXC* GetDXC() { return &dxc_; }

	/// <summary>
	/// (3D描画用)描画ターゲット追加関数
	/// </summary>
	/// <param name="view">カメラデータまでのアドレス</param>
	/// <param name="backBuffer">BackBufferリソース</param>
	/// <param name="depth">DSVバッファリソース</param>
	void AddTarget(const D3D12_GPU_VIRTUAL_ADDRESS& view, BackBuffer* backBuffer, DepthStencil* depthBuffer) { 
		normalRenderer_.AddTarget({ view, backBuffer, depthBuffer }); 
	}

	/// <summary>
	/// (2D描画用)描画ターゲット追加関数
	/// </summary>
	/// <param name="view">カメラデータまでのアドレス</param>
	/// <param name="backBuffer">BackBufferリソース</param>
	/// <param name="depth">DSVバッファリソース</param>
	void Add2DTarget(const D3D12_GPU_VIRTUAL_ADDRESS& view, BackBuffer* backBuffer, DepthStencil* depthBuffer) { 
		spriteRenderer_.AddTarget({ view, backBuffer, depthBuffer }); 
	}
	
	/// <summary>
	/// (ポストプロセス用)描画ターゲット追加関数
	/// </summary>
	/// <param name="renderer">ポストプロセスを掛けた結果を格納するリソース</param>
	/// <param name="texture">ポストプロセスを掛けるテクスチャ</param>
	/// <param name="depthBuffer">深度リソース</param>
	/// <param name="view">ポストプロセスパラメーターまでのアドレス</param>
	void AddTarget(RenderResource* renderer, BackBuffer* texture, DepthStencil* depthBuffer, PostProcessor* pp) { ppRenderer_.AddTarget({ renderer, texture, depthBuffer, pp }); }

	/// <summary>
	/// コマンド管理クラスゲッター
	/// </summary>
	/// <returns>コマンド管理クラス</returns>
	Command* GetCommand() { return &command_; }

	/// <summary>
	/// 平行光源ゲッター
	/// </summary>
	/// <returns>平行光源本体</returns>
	DirectionalLight* GetDirectionalLight() { return light_.get(); }

private: // プライベートなメンバ関数

	/// <summary>
	/// ImGui描画関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	void ImGuiDraw(ID3D12GraphicsCommandList6* list);

private: // メンバ変数

	// コマンド管理用
	Command command_;

	// DirectXシェーダーコンパイラ
	DXC dxc_;

	// SRV
	SRV* srv_;

	// 形状マネージャ
	ModelManager* modelManager_ = nullptr;

	// 平行光源
	// ! 今は仮置き、じきにライト用の管理マネージャを作成する
	std::unique_ptr<DirectionalLight> light_;

	// 3D描画レンダラー
	ModelRenderer normalRenderer_;

	// ポストプロセス描画レンダラー
	PPRenderer ppRenderer_;

	// 2D描画レンダラー
	SpriteRenderer spriteRenderer_;

};

