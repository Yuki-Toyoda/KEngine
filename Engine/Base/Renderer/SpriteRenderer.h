#pragma once
#include "IRenderer.h"
#include "Engine/Base/Resource/Rendering/BackBuffer.h"
#include "Engine/Base/Resource/Rendering/DepthStencil.h"
#include "Engine/Base/DXC/DXC.h"

// クラスの前方宣言
class PrimitiveManager;
class ModelManager;

/// <summary>
/// 2D描画用レンダラークラス
/// </summary>
class SpriteRenderer : public IRenderer
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
	SpriteRenderer() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SpriteRenderer() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="dxc">DirectXシェーダーコンパイラ</param>
	/// <param name="pm">モデルマネージャ</param>
	void Init(DirectXDevice* device, DXC* dxc, ModelManager* mm);

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
	ID3D12RootSignature* spriteRootSignature_ = nullptr; // スプライト用

	// PSO群
	PSO spritePSO_;	// スプライト描画

	// 描画ターゲット
	std::vector<Target> targets_;

	// モデルマネージャー
	ModelManager* modelManager_ = nullptr;

};

