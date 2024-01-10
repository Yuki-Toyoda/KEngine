#pragma once
#include <memory>
#include <map>
#include <string>
#include <wrl.h>

#include "Texture.h"

/// <summary>
/// テクスチャマネージャー
/// </summary>
class TextureManager
{
private:  // コンストラクタ等

	// シングルトンパターンの設定
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;

public: // 静的なメンバ関数

	/// <summary>
	/// シングルトンインスタンスの取得
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static TextureManager* GetInstance();

	/// <summary>
	/// テクスチャロード関数(ディレクトリ指定なし)
	/// </summary>
	/// <param name="fileName">Resources内のファイル名</param>
	/// <returns>テクスチャ</returns>
	static Texture* Load(std::string fileName);

	/// <summary>
	/// テクスチャロード関数
	/// </summary>
	/// <param name="directoryPath">テクスチャまでのディレクトリパス</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャ</returns>
	static Texture* Load(std::string directoryPath, std::string fileName);

public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	void Init();

private: // 読み込み関数群

	/// <summary>
	/// テクスチャロード関数(ディレクトリ指定なし)
	/// </summary>
	/// <param name="fileName">Resources内のファイル名</param>
	/// <returns>テクスチャ</returns>
	Texture* LoadTexture(std::string fileName);

	/// <summary>
	/// テクスチャロード関数
	/// </summary>
	/// <param name="directoryPath">テクスチャまでのディレクトリパス</param>
	/// <param name="fileName">ファイル名</param>
	/// <returns>テクスチャ</returns>
	Texture* LoadTexture(std::string directoryPath, std::string fileName);

private: // メンバ変数

	// DirectX12汎用クラス
	DirectXCommon* dxCommon_;

	// テクスチャコンテナ
	std::map<std::string, std::unique_ptr<Texture>> textures_;

};

