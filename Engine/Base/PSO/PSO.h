#pragma once
#include "../Device/DirectXDevice.h"
#include "../../Debug/Debug.h"

#include <cassert>
#include <string>
#include <dxcapi.h>

/// <summary>
/// DXC構造体
/// </summary>
struct DXC {
	Microsoft::WRL::ComPtr<IDxcUtils> dxcUtils_;				   // dxcUtils
	Microsoft::WRL::ComPtr<IDxcCompiler3> dxcCompiler_;			   // dxcコンパイラ
	Microsoft::WRL::ComPtr<IDxcIncludeHandler> dxcIncludeHandler_; // InludeHandler
};

/// <summary>
/// パイプラインステートオブジェクトクラス
/// </summary>
class PSO final{
public: // メンバ関数
	
	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="signature">ルートシグネチャ</param>
	/// <param name="dxc">dxc構造体</param>
	/// <param name="vs">使用する頂点シェーダまでのファイルパス</param>
	/// <param name="ps">使用するピクセルシェーダまでのファイルパス</param>
	/// <param name="blendType">ブレンド設定</param>
	/// <param name="wire">ワイヤーフレーム状態にするか 0 : 通常表示 ** 1 : ワイヤー表示</param>
	void Init(
		ID3D12Device* device, 
		ID3D12RootSignature* signature, 
		DXC* dxc, 
		std::wstring vs, 
		std::wstring ps, 
		int blendType,
		bool isWriteDSV,
		UINT wire);

private: // プライベートなメンバ関数

	/// <summary>
	/// ブレンド設定を行う関数
	/// </summary>
	/// <returns>ブレンド設定</returns>
	D3D12_BLEND_DESC SettingBlendState(int state);
	/// <summary>
	/// ラスタライザ設定を行う関数
	/// </summary>
	/// <param name="wire">ワイヤーフレーム表示</param>
	/// <returns>ラスタライザ設定</returns>
	D3D12_RASTERIZER_DESC SettingRasterizerDesc(UINT wire);
	/// <summary>
	/// 深度ステンシルビュー設定を行う関数
	/// </summary>
	/// <param name="isWriteDSV">Depthを書き込むか</param>
	/// <returns>深度ステンシルビュー設定</returns>
	D3D12_DEPTH_STENCIL_DESC SettingDepthStencilState(bool isWriteDSV = false);

	/// <summary>
	/// 頂点シェーダ生成関数
	/// </summary>
	/// <param name="dxc">HLSLコード変換用</param>
	/// <param name="vs">頂点シェーダまでのファイルパス</param>
	/// <returns>頂点シェーダのバイナリオブジェクト</returns>
	IDxcBlob* CreateVertexShader(DXC* dxc, std::wstring vs);
	/// <summary>
	/// ピクセルシェーダ生成関数
	/// </summary>
	/// <param name="dxc">HLSLコード変換用</param>
	/// <param name="ps">ピクセルシェーダまでのファイルパス</param>
	/// <returns>ピクセルシェーダのバイナリオブジェクト</returns>
	IDxcBlob* CreatePixelShader(DXC* dxc, std::wstring ps);

	/// <summary>
	/// シェーダーのコンパイルを行う関数
	/// </summary>
	/// <param name="filePath">compilerするSharderファイルへのパス</param>
	/// <param name="profile">compilerに使用するprofile</param>
	/// <param name="dxcUtils">dxcUtils</param>
	/// <param name="dxcCompiler">dxcCompiler</param>
	/// <param name="includeHandler">includeHandler</param>
	/// <returns>コンパイル済みシェーダーのバイナリオブジェクト</returns>
	IDxcBlob* CompileShader(const std::wstring& filePath, const wchar_t* profile, DXC* dxc);

public: // パブリックなメンバ変数
	// パイプラインステートオブジェクトの状態
	Microsoft::WRL::ComPtr<ID3D12PipelineState> state_;
};

