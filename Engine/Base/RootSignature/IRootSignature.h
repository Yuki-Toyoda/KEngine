#pragma once
#include <d3d12.h>
#include <stdint.h>
#include <wrl.h>
#include <vector>
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
	/// 共通初期化関数
	/// </summary>
	/// <param name="rootParameterCount">生成するルートパラメータ数</param>
	void PreInit(const int& rootParameterCount);

	/// <summary>
	/// 定数バッファ用のパラメータを生成する関数
	/// </summary>
	/// <param name="ShaderRegisterCount">hlsl上の登録番号</param>
	/// <param name="visibility">頂点シェーダーかピクセルシェーダーどちらで使うか</param>
	void CreateCBVParameter(const int& ShaderRegisterCount, const D3D12_SHADER_VISIBILITY& visibility);

	/// <summary>
	/// ディスクリプタテーブル用のパラメータを生成する関数
	/// </summary>
	/// <param name="ShaderRegisterCount">hlsl上の登録番号</param>
	/// <param name="visibility">頂点シェーダーかピクセルシェーダーどちらで使うか</param>
	/// <param name="type">テーブルのレンジタイプ</param>
	/// <param name="numDescriptor">ディスクリプタ数</param>
	void CreateDescriptorTableParameter(const int& ShaderRegisterCount, const D3D12_SHADER_VISIBILITY& visibility, const D3D12_DESCRIPTOR_RANGE_TYPE& type = D3D12_DESCRIPTOR_RANGE_TYPE_SRV, const int& numDescriptor = 1);

	/// <summary>
	/// ルートシグネチャ生成関数
	/// </summary>
	void CreateRootSignature();

public: // 純粋仮想関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Init();

private: // メンバ変数

	// デバイス
	ID3D12Device2* device_;

	// ルートシグネチャ本体
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;

	// ルートパラメータ配列
	std::vector<D3D12_ROOT_PARAMETER> rootParameters_;
	// ルートシグネチャ設定用構造体
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_;
};

/// <summary>
/// IRootSignatureを継承したクラスを選択できるテンプレート
/// </summary>
template <class SelectRootSignature>
concept IsIRootSignature = std::is_base_of<IRootSignature, SelectRootSignature>::value;
