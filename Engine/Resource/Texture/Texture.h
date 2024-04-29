#pragma once
#include <string>
#include "../../../Externals/DirectXTex/DirectXTex.h"

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "../../Math/Vector2.h"

// クラスの前方宣言
class DirectXCommon;
class TextureResource;

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

	// デストラクタ
	~Texture() = default;

public: // アクセッサ等

	/// <summary>
	/// インデックス情報ゲッター
	/// </summary>
	/// <returns>コマンドマネージャー上でのインデックス情報(定数値)</returns>
	int GetIndex() const { return index_; }

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

private: // メンバ変数

	// SRV上のインデックス
	int index_ = -1;

	// テクスチャ解像度
	Vector2 size_;
};

