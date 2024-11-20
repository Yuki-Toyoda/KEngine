#pragma once
#include <string>
#include <Windows.h>
#include "Externals/DirectXTex/DirectXTex.h"

/// <summary>
/// テクスチャの形式を変更するコンバーター
/// </summary>
class TextureConverter
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	TextureConverter() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TextureConverter() = default;

public: // メンバ関数

	/// <summary>
	/// WICテクスチャをDDSテクスチャに変換する
	/// </summary>
	/// <param name="filePath">テクスチャまでのファイルパス</param>
	void ConvertTextureWICToDDS(const std::string& filePath);

private: // 機能関数群

	/// <summary>
	/// WICテクスチャファイルの読み込み
	/// </summary>
	/// <param name="filePath">テクスチャまでのファイルパス</param>
	void LoadWICTextureFromFile(const std::string& filePath);

	/// <summary>
	/// 渡されたファイルパスをファイルパス / ファイル名に分割する
	/// </summary>
	/// <param name="filePath">分割を行うファイルパス</param>
	void SeparateFilePath(const std::wstring& filePath);

	/// <summary>
	/// DDSテクスチャとしてファイルに書き出す関数
	/// </summary>
	void SaveDDSTextureToFile();

private: // 静的機能関数群

	/// <summary>
	/// マルチバイト文字列をワイド文字列に変換する
	/// </summary>
	/// <param name="mString">変更する文字列</param>
	/// <returns></returns>
	static std::wstring ConvertMultiByteStringToWideString(const std::string& mString);

private: // メンバ変数

	// テクスチャのメタデータ
	DirectX::TexMetadata metadata_{};
	// イメージ
	DirectX::ScratchImage scratchImage_{};

	// ディレクトリパス
	std::wstring directoryPath_{};
	// ファイル名
	std::wstring fileName_{};
	// ファイル拡張子
	std::wstring fileExt_{};

};

