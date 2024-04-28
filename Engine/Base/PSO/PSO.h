#pragma once
#include "../Device/DirectXDevice.h"
#include "../../Debug/Debug.h"
#include "../DXC/DXC.h"

/// <summary>
/// パイプラインステートオブジェクトクラス
/// </summary>
class PSO{
public: // メンバ関数

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="signature">RootSignature</param>
	/// <param name="dxc">DirectXシェーダーコンパイラ</param>
	PSO& Init(ID3D12RootSignature* signature, DXC* dxc);

	/// <summary>
	/// ブレンド設定セット関数
	/// </summary>
	/// <param name="state">ブレンド設定</param>
	/// <returns>PSO自身</returns>
	PSO& SetBlendState(int state);

	/// <summary>
	/// ラスタライザ設定セット関数
	/// </summary>
	/// <param name="cullMode">カリングモード</param>
	/// <param name="fillMode">形状のフィルモード</param>
	/// <returns>PSO自身</returns>
	PSO& SetRasterizerState(
		D3D12_CULL_MODE cullMode = D3D12_CULL_MODE::D3D12_CULL_MODE_BACK,
		D3D12_FILL_MODE fillMode = D3D12_FILL_MODE::D3D12_FILL_MODE_SOLID);

	/// <summary>
	/// メッシュシェーダーセット関数
	/// </summary>
	/// <param name="filePath">メッシュシェーダーまでのファイルパス</param>
	/// <returns>PSO自身</returns>
	PSO& SetMeshShader(std::string filePath);

	/// <summary>
	/// ピクセルシェーダーセット関数
	/// </summary>
	/// <param name="filePath">ピクセルシェーダーまでのファイルパス</param>
	/// <returns>PSO自身</returns>
	PSO& SetPixelShader(std::string filePath);

	/// <summary>
	/// DSV設定セット関数
	/// </summary>
	/// <param name="writeDSV">深度情報を書き込むか</param>
	/// <returns>PSO自身</returns>
	PSO& SetDepthStencilState(bool writeDSV);

	/// <summary>
	/// DSVフォーマットセット関数
	/// </summary>
	/// <param name="format">DSVのフォーマット</param>
	/// <returns>DSVのフォーマット</returns>
	PSO& SetDSVFormat(DXGI_FORMAT format);

	/// <summary>
	/// PSO作成関数
	/// </summary>
	/// <param name="device">デバイス</param>
	void Build(ID3D12Device2* device);

public: // アクセッサ等

	/// <summary>
	/// PSOゲッター
	/// </summary>
	/// <returns>PSO</returns>
	ID3D12PipelineState* GetState() { return state_.Get(); }

private: // プライベートなメンバ関数

	/// <summary>
	/// ブレンド設定を行う関数
	/// </summary>
	/// <returns>ブレンド設定</returns>
	D3D12_BLEND_DESC SettingBlendState(int state);
	/// <summary>
	/// ラスタライザ設定を行う関数
	/// </summary>
	/// <returns>ラスタライザ設定</returns>
	D3D12_RASTERIZER_DESC SettingRasterizerDesc();
	/// <summary>
	/// 深度ステンシルビュー設定を行う関数
	/// </summary>
	/// <param name="isWriteDSV">Depthを書き込むか</param>
	/// <returns>深度ステンシルビュー設定</returns>
	D3D12_DEPTH_STENCIL_DESC SettingDepthStencilState(bool isWriteDSV = false);

private: // メンバ変数
	// パイプラインステートオブジェクトの状態
	Microsoft::WRL::ComPtr<ID3D12PipelineState> state_;
	// PSOの設定用構造体
	D3DX12_MESH_SHADER_PIPELINE_STATE_DESC desc_{};

	// シェーダーコンパイラ
	DXC* dxc_ = nullptr;
};

