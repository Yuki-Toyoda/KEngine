#pragma once
#include "../GameObject/WorldTransform.h"
#include "../Math/Vector4.h"

#include "../Resource/Texture/Texture.h"

#include "../Base/Resource/Data/ConstantBuffer.h"
#include "../Base/Resource/Data/StructuredBuffer.h"

#include "../../Externals/imgui/imgui.h"

#include <array>
#include <d3d12.h>
#include <string>
#include <cassert>

/// <summary>
/// スカイボックスクラス
/// </summary>
class SkyBox
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	SkyBox() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SkyBox() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="ddsPath">使用するDDS形式のテクスチャまでのファイルパス</param>
	/// <param name="ddsName">称するDDS形式のテクスチャのファイル名</param>
	void Init(const std::string& ddsPath, const std::string& ddsName);

	/// <summary>
	/// 描画関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw(ID3D12GraphicsCommandList6* cmdList);

	/// <summary>
	/// ImGui表示関数
	/// </summary>
	/// <param name="id">ImGui上で表示されるID</param>
	void DisplayImGui(const std::string& id);

public: // アクセッサ等

	/// <summary>
	/// テクスチャのGPUアドレスのゲッター
	/// </summary>
	/// <returns>テクスチャのGPUアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureAddress() { return texture_.GetAddress(); }

private: // メンバ変数

	// トランスフォーム
	WorldTransform transform_{};

	// スカイボックスに使用するテクスチャデータ
	Texture texture_{};

	// バッファ変数群
	std::unique_ptr<ConstantBuffer<Matrix4x4>> transformBuffer_; // トランスフォームバッファ
	std::unique_ptr<StructuredBuffer<Vector4>> vertexBuffer_;	 // 頂点バッファ
	std::unique_ptr<StructuredBuffer<int32_t>> indexBuffer_;	 // インデックスバッファ
};

