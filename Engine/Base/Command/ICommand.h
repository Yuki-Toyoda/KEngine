#pragma once
#include <memory>
#include "../Device/DirectXDevice.h"
#include "../PSO/PSO.h"

// クラスの前方宣言
class RTV; // レンダーターゲットビュー
class SRV; // シェーダーリソースビュー
class DSV; // 深度ステンシルビュー

/// <summary>
/// 描画コマンド基底クラス
/// </summary>
class ICommand
{
public: // メンバ関数

	// コンストラクタ
	ICommand() = default;
	// デストラクタ
	~ICommand() = default;

	/// <summary>
	/// 初期化関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	/// <param name="dxc">hlslコード変換用</param>
	/// <param name="signature">ルートシグネチャ</param>
	/// <param name="resource">インデックスバッファリソース</param>
	/// <param name="vs">使用する頂点シェーダ</param>
	/// <param name="ps">使用するピクセルシェーダ</param>
	void Init(ID3D12Device2* device, DXC* dxc, ID3D12RootSignature* signature, int32_t blendType = 0);

	/// <summary>
	/// 描画前処理関数
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	virtual void PreDraw(ID3D12GraphicsCommandList* cmdList) = 0;
	/// <summary>
	/// 描画後処理関数
	/// </summary>
	/// <param name="cmdList"></param>
	virtual void PostDraw(ID3D12GraphicsCommandList* cmdList) = 0;

	/// <summary>
	/// パイプラインステートオブジェクトの生成関数
	/// </summary>
	/// <param name="device">DirectX12のデバイス</param>
	/// <param name="dxc">hlslコード変換用</param>
	/// <param name="signature">ルートシグネチャ</param>
	/// <param name="blendType">設定するブレンドタイプ</param>
	virtual void CreatePSO(ID3D12Device2* device, DXC* dxc, ID3D12RootSignature* signature, int32_t blendType = 0);

	/// <summary>
	/// リソースバリア設定関数
	/// </summary>
	/// <param name="resource">バリアを張るリソース</param>
	/// <param name="stateBefore">遷移前のリソース状態</param>
	/// <param name="stateAfter">遷移後のリソース状態</param>
	/// <returns>リソースバリアの設定</returns>
	D3D12_RESOURCE_BARRIER SettingResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES stateBefore, D3D12_RESOURCE_STATES stateAfter);

public: // アクセッサ等

	/// <summary>
	/// ディスクリプタヒープセッター
	/// </summary>
	/// <param name="rtv">レンダーターゲットビュー</param>
	/// <param name="srv">シェーダーリソースビュー</param>
	/// <param name="dsv">深度ステンシルビュー</param>
	void SetDescriptorHeap(RTV* rtv, SRV* srv, DSV* dsv);

	/// <summary>
	/// パイプラインステートオブジェクトゲッター
	/// </summary>
	/// <returns>指定したパイプライン</returns>
	ID3D12PipelineState* GetPSOState() { return pso_->state_.Get(); }

	/// <summary>
	/// シェーダーのバイナリオブジェクトゲッター
	/// </summary>
	/// <returns>シェーダーのバイナリオブジェクト</returns>
	IDxcBlob* GetShaderBlob() { return pso_->meshShaderBlob_.Get(); }

protected: // 継承先メンバ変数

	// ディスクリプタヒープの本体
	RTV* rtv_ = nullptr; // レンダーターゲットビュー
	SRV* srv_ = nullptr; // シェーダーリソースビュー
	DSV* dsv_ = nullptr; // 深度ステンシルビュー

	// パイプラインステートオブジェクトの定義
	std::unique_ptr<PSO> pso_;

};

