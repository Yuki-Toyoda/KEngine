#pragma once
#include <string>
#include "../../../Externals/DirectXTex/DirectXTex.h"

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "../../Math/Vector2.h"

// クラスの前方宣言
class DirectXCommon;
class TextureResource;
class RenderResource;

/// <summary>
/// テクスチャ
/// </summary>
class Texture final
{
public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	Texture() = default;

	/// <summary>
	/// テクスチャリソースを直接代入するコンストラクタ
	/// </summary>
	/// <param name="resource">代入するテクスチャリソース</param>
	Texture(const TextureResource& resource);

	/// <summary>
	/// レンダリングリソースを直接代入するコンストラクタ
	/// </summary>
	/// <param name="resource">代入するレンダリングリソース</param>
	Texture(const RenderResource& resource);

	// デストラクタ
	~Texture() = default;

public: // アクセッサ等

	/// <summary>
	/// GPU上のアドレスゲッター
	/// </summary>
	/// <returns>GPU上のアドレス</returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetView_() { return view_; }

	/// <summary>
	/// テクスチャサイズゲッター
	/// </summary>
	/// <returns>テクスチャサイズ</returns>
	const Vector2 GetTextureSize() const { return size_; }

public: // 演算子オーバーロード

	/// <summary>
	/// =演算子オーバーロード
	/// </summary>
	/// <param name="resource">代入するリソース</param>
	/// <returns>テクスチャ</returns>
	Texture& operator=(const TextureResource& resource);

	/// <summary>
	/// =演算子オーバーロード
	/// </summary>
	/// <param name="resource">代入するリソース</param>
	/// <returns>テクスチャ</returns>
	Texture& operator=(const RenderResource& resource);

private: // メンバ変数

	// GPU上のアドレス
	D3D12_GPU_DESCRIPTOR_HANDLE view_;

	// テクスチャ解像度
	Vector2 size_;
};

