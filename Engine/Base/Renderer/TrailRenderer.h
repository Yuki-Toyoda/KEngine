#pragma once
#include "IRenderer.h"
#include "Engine/Base/Resource/Rendering/BackBuffer.h"
#include "Engine/Base/Resource/Rendering/DepthStencil.h"
#include "Engine/Base/DXC/DXC.h"
#include <functional>

/// <summary>
/// 軌跡描画用レンダラー
/// </summary>
class TrailRenderer : public IRenderer
{
public: // サブクラス

	/// <summary>
	/// 描画ターゲット構造体
	/// </summary>
	struct Target {
		D3D12_GPU_VIRTUAL_ADDRESS view_;	// カメラデータのGPU上のアドレス
		BackBuffer* backBuffer_;			// バックバッファリソース
		DepthStencil* depthBuffer_;			// DSVバッファリソース
	};

public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TrailRenderer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TrailRenderer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxc">DirectXシェーダーコンパイラ</param>
	/// <param name="pm">モデルマネージャ</param>
	void Init(DirectXDevice* device, DXC* dxc);

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

	/// <summary>
	/// 描画タスクの追加
	/// </summary>
	/// <param name="func">描画関数</param>
	void AddDrawTask(std::function<void(ID3D12GraphicsCommandList6* cmdList)> func) { drawFunctions_.push_back(func); }

private: // メンバ変数

	// ルートシグネチャ
	IRootSignature rootSignature_;
	// PSO
	PSO pso_;

	// 描画ターゲット
	std::vector<Target> targets_;

	// 描画関数配列
	std::vector<std::function<void(ID3D12GraphicsCommandList6* cmdList)>> drawFunctions_;
};

