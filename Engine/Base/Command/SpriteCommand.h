#pragma once
#include "BaseCommand.h"

class SpriteCommand : public BaseCommand
{
public: // メンバ関数

	/// <summary>
	/// 描画前処理関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PreDraw(ID3D12GraphicsCommandList* cmdList) override;
	/// <summary>
	/// 描画後処理関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void PostDraw(ID3D12GraphicsCommandList* cmdList) override;

private: // プライベートなメンバ関数
	/// <summary>
	/// パイプラインステートオブジェクトの生成関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	/// <param name="dxc">hlslコード変換用</param>
	/// <param name="signature">ルートシグネチャ</param>
	/// <param name="vs">使用する頂点シェーダまでのファイルパス</param>
	/// <param name="ps">使用するピクセルシェーダまでのファイルパス</param>
	/// <param name="wire">ワイヤーフレーム表示</param>
	void CreatePSO(ID3D12Device* device, DXC* dxc, ID3D12RootSignature* signature, std::wstring vs, std::wstring ps, int32_t blendType = 0, UINT wire = 0) override;
};

