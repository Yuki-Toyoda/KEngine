#pragma once
#include <string>
#include "../../../Externals/DirectXTex/DirectXTex.h"

#include <d3d12.h>
#pragma comment(lib, "d3d12.lib")

#include "../../Math/Vector2.h"

// クラスの前方宣言
class DirectXCommon;
class CommandManager;

/// <summary>
/// テクスチャ
/// </summary>
class Texture final
{
public: // メンバ関数

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="manager">描画コマンドマネージャー</param>
	/// <param name="filePath">テクスチャまでのファイルパス</param>
	Texture(CommandManager* manager, const std::string filePath);
	// デストラクタ
	~Texture() = default;

public: // アクセッサ等
	/// <summary>
	/// インデックス情報ゲッター
	/// </summary>
	/// <returns>コマンドマネージャー上でのインデックス情報(定数値)</returns>
	int GetIndex() const { return index_; }
	/// <summary>
	/// インデックス情報セッター
	/// </summary>
	/// <param name="index">設定するインデックス</param>
	void SetIndex(int index) { index_ = index; }

	/// <summary>
	/// テクスチャサイズゲッター
	/// </summary>
	/// <returns>テクスチャサイズ</returns>
	const Vector2 GetTextureSize() const { return { (float)mipImages_.GetImages()->width, (float)mipImages_.GetImages()->height }; }

private: // プライベートなメンバ関数

	/// <summary>
	/// テクスチャのロード関数
	/// </summary>
	/// <param name="filePath">テクスチャまでのファイルパス</param>
	void Load(const std::string& filePath);

private: // メンバ変数

	// コマンドマネージャー上でのインデックス情報
	int index_;

	// ミップマップ付きのテクスチャ
	DirectX::ScratchImage mipImages_;
};

