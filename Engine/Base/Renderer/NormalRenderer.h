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
	/// <param name="dxc">DirectXシェーダーコンパイラ</param>
	/// <param name="pm">形状マネージャ</param>
	/// <param name="lt">平行光源</param>
	void Init(DirectXDevice* device, DXC* dxc, PrimitiveManager* pm, DirectionalLight* lt);

	/// <summary>
	/// 描画命令関数
	/// </summary>
	/// <param name="list">コマンドリスト</param>
	void DrawCall(ID3D12GraphicsCommandList6* list);

	/// <summary>
	/// レンダラーリセット関数
	/// </summary>
	void Reset();

public: // アクセッサ等

	/// <summary>
	/// 描画ターゲット追加関数
	/// </summary>
	/// <param name="target">描画ターゲット</param>
	void AddTarget(const Target& target) { targets_.push_back(target); }

private: // メンバ変数

	// ルートシグネチャ群
	ID3D12RootSignature* standardRootSignature_ = nullptr; // 通常描画用
	ID3D12RootSignature* skinRootSignature_ = nullptr;	   // スキンアニメーション用

	// PSO群
	PSO standardPSO_;  // 通常描画
	PSO skinModelPSO_; // スキンアニメーション描画

	// 描画ターゲット
	std::vector<Target> targets_;

	// 形状マネージャ
	PrimitiveManager* primitiveManager_ = nullptr;

	// 平行光源取得用
	DirectionalLight* light_ = nullptr;

};

