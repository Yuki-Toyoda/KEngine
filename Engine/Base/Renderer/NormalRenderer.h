#pragma once
#include "IRenderer.h"
#include "../Resource/Rendering/BackBuffer.h"
#include "../Resource/Rendering/DepthStencil.h"
#include "../DXC/DXC.h"

// クラスの前方宣言
class PrimitiveManager;
class DirectionalLight;

/// <summary>
/// 通常描画用レンダラークラス
/// </summary>
class NormalRenderer : public IRenderer
{
public: // サブクラス

	/// <summary>
	/// 描画ターゲット構造体
	/// </summary>
	struct Target {
		D3D12_GPU_VIRTUAL_ADDRESS view_;		// カメラデータのGPU上のアドレス
		BackBuffer*				  backBuffer_;  // バックバッファリソース
		DepthStencil*			  depthBuffer_; // DSVバッファリソース
	};

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
	/// <param name="pm">形状マネージャ</param>
	/// <param name="lt">平行光源</param>
	void Init(DirectXDevice* device, ID3D12RootSignature* signature, DXC* dxc, PrimitiveManager* pm, DirectionalLight* lt);

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

	// パイプラインステートオブジェクト
	PSO pso_;

	// 描画ターゲット
	Target target_;

	// 形状マネージャ
	PrimitiveManager* primitiveManager_ = nullptr;

	// 平行光源取得用
	DirectionalLight* light_ = nullptr;

};

