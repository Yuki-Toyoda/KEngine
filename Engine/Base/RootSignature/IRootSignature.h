#pragma once
#include <d3d12.h>
#include <stdint.h>
#include <wrl.h>
#include <vector>
#include <list>
#include <cassert>
#include "../../Debug/Debug.h"

/// <summary>
/// ルートシグネチャ基底クラス
/// </summary>
class IRootSignature
{
public: // コンストラクタ等

	/// <summary>
	/// コンストラクタ
	/// </summary>
	IRootSignature() = default;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IRootSignature() = default;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

	/// <summary>
	/// 定数バッファ用のパラメータを生成する関数
	/// </summary>
	/// <param name="ShaderRegisterCount">hlsl上の登録番号</param>
	/// <param name="visibility">頂点シェーダーかピクセルシェーダーどちらで使うか</param>
	void CreateCBVParameter(const int ShaderRegisterCount, const D3D12_SHADER_VISIBILITY visibility);

	/// <summary>
	/// ディスクリプタテーブル用のパラメータを生成する関数
	/// </summary>
	/// <param name="ShaderRegisterCount">hlsl上の登録番号</param>
	/// <param name="visibility">頂点シェーダーかピクセルシェーダーどちらで使うか</param>
	/// <param name="type">テーブルのレンジタイプ</param>
	/// <param name="numDescriptor">ディスクリプタ数</param>
	void CreateDescriptorTableParameter(const int ShaderRegisterCount, const D3D12_SHADER_VISIBILITY visibility, const D3D12_DESCRIPTOR_RANGE_TYPE type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV, const int numDescriptor = 1);

	/// <summary>
	/// サンプラー追加関数
	/// </summary>
	/// <param name="ShaderRegisterCount">hlsl上の登録番号</param>
	/// <param name="visibility">頂点シェーダーかピクセルシェーダーどちらで使うか</param>
	/// <param name="filter">フィルター（default:バイオリニアフィルタ）</param>
	/// <param name="func">比較関数（default:比較しない）</param>
	/// <param name="addressU">範囲外をどうするかの設定</param>
	/// <param name="addressV">範囲外をどうするかの設定</param>
	/// <param name="addressW">範囲外をどうするかの設定</param>
	void CreateSampler(
		const int ShaderRegisterCount,
		const D3D12_SHADER_VISIBILITY visibility = D3D12_SHADER_VISIBILITY_PIXEL,
		D3D12_FILTER filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR,					// フィルター（default:バイオリニアフィルタ）
		D3D12_COMPARISON_FUNC func = D3D12_COMPARISON_FUNC_NEVER,				// 比較関数（default:比較しない）
		D3D12_TEXTURE_ADDRESS_MODE addressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// 範囲外をどうするかの設定
		D3D12_TEXTURE_ADDRESS_MODE addressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP,	// 範囲外をどうするかの設定
		D3D12_TEXTURE_ADDRESS_MODE addressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP	// 範囲外をどうするかの設定)
	);

	/// <summary>
	/// ルートシグネチャ生成関数
	/// </summary>
	void CreateRootSignature();

public: // アクセッサ等

	/// <summary>
	/// デバイスセッター
	/// </summary>
	/// <param name="device">デバイス</param>
	void SetDevice(ID3D12Device2* device) { device_ = device; }

	/// <summary>
	/// ルートシグネチャゲッター
	/// </summary>
	/// <returns>ルートシグネチャ本体</returns>
	ID3D12RootSignature* GetRootSignature() { return rootSignature_.Get(); }

protected: // メンバ変数

	// デバイス
	ID3D12Device2* device_ = nullptr;

	// ルートシグネチャ本体
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	// ルートパラメータ配列
	std::vector<D3D12_ROOT_PARAMETER> rootParameters_;
	// 配列用設定用構造体配列
	std::list<D3D12_DESCRIPTOR_RANGE> descriptorRangeDescs_;
	// サンプラー設定酔う構造体配列
	std::vector<D3D12_STATIC_SAMPLER_DESC> samplers_;
	// ルートシグネチャ設定用構造体
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
};

/// <summary>
/// IRootSignatureを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectRootSignature>
concept IsIRootSignature = std::is_base_of<IRootSignature, SelectRootSignature>::value;
