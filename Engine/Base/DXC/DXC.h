#pragma once
#include <wrl.h>
#include <dxcapi.h>
#pragma comment(lib,"dxcompiler.lib")

#include <cassert>
#include <string>

/// <summary>
/// DirectXシェーダーコンパイラ
/// </summary>
class DXC final
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	DXC();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~DXC() = default;

public: // メンバ関数

	/// <summary>
	/// シェーダーコンパイル関数
	/// </summary>
	/// <param name="filePath">シェーダーまでのファイルパス</param>
	/// <param name="profile">シェーダーのプロファイル</param>
	/// <returns>コンパイル済みシェーダーのバイナリオブジェクト</returns>
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile);


public: // アクセッサ等

	/// <summary>
	/// dxcUtilsゲッター
	/// </summary>
	/// <returns>dxcUtils</returns>
	IDxcUtils* Utils() { return utils_.Get(); }

	/// <summary>
	/// dxcコンパイラゲッター
	/// </summary>
	/// <returns>dxcコンパイラ</returns>
	IDxcCompiler3* Compiler() { return compiler_.Get(); }

	/// <summary>
	/// Includeハンドラーゲッター
	/// </summary>
	/// <returns>Includeハンドラー</returns>
	IDxcIncludeHandler* IncludeHandler() { return includeHandler_.Get(); }

private: // メンバ変数

	Microsoft::WRL::ComPtr<IDxcUtils> utils_;					// dxcUtils
	Microsoft::WRL::ComPtr<IDxcCompiler3> compiler_;			// dxcコンパイラ
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> includeHandler_; // includeハンドラー

};

