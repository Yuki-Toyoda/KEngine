#pragma once
#include "PSO.h"

/// <summary>
/// メッシュシェーダー用PSOクラス
/// </summary>
class MeshletPSO : public PSO {
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
		ID3D12Device2* device,
		ID3D12RootSignature* signature,
		DXC* dxc,
		std::wstring vs,
		std::wstring ps,
		int blendType,
		bool isWriteDSV,
		UINT wire);

private: // プライベートなメンバ関数

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
	/// メッシュシェーダ生成関数
	/// </summary>
	/// <param name="dxc">HLSLコード変換用</param>
	/// <param name="ms">メッシュシェーダまでのファイルパス</param>
	/// <returns>メッシュシェーダのバイナリオブジェクト</returns>
	IDxcBlob* CreateMeshShader(DXC* dxc, std::wstring ms);

};

