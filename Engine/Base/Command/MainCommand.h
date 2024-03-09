#pragma once
#include "ICommand.h"

/// <summary>
/// メイン描画コマンドクラス
/// </summary>
class MainCommand : public ICommand
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
	void CreatePSO(ID3D12Device2* device, DXC* dxc, ID3D12RootSignature* signature, int32_t blendType = 0) override;
};

